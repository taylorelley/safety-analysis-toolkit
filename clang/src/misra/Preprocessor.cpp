// SPDX-License-Identifier: Apache-2.0
// Rules 20.x: preprocessing directives.
#include "Misra.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/MacroInfo.h"
#include <map>
#include <set>

using namespace clang;

namespace safety::misra {
namespace {
using Check = std::function<std::unique_ptr<PPCallbacks>(Rule &, Preprocessor &)>;
Spec ppSpec(Check Make) {
  Spec S;
  S.Preprocessor = [Make](Rule &R, Preprocessor &PP, const SourceManager &) { PP.addPPCallbacks(Make(R, PP)); };
  return S;
}
bool user(Preprocessor &PP, SourceLocation L) { return isUserLocation(PP.getSourceManager(), L); }

// Raw text of the directive line that starts at Loc, with line splices joined.
std::string directiveLine(const SourceManager &SM, SourceLocation Loc) {
  bool Invalid = false;
  const char *Start = SM.getCharacterData(SM.getSpellingLoc(Loc), &Invalid);
  if (Invalid) return "";
  std::string Out;
  for (const char *P = Start; *P && *P != '\n'; ++P) {
    if (*P == '\\' && (P[1] == '\n' || (P[1] == '\r' && P[2] == '\n'))) {
      P += P[1] == '\r' ? 2 : 1;
      continue;
    }
    Out.push_back(*P);
  }
  return Out;
}
struct RawToken {
  tok::TokenKind Kind;
  std::string Text;
};
std::vector<RawToken> rawTokens(const LangOptions &LO, llvm::StringRef Text) {
  std::vector<RawToken> Out;
  std::string Buffer = Text.str();
  Lexer L(SourceLocation(), LO, Buffer.data(), Buffer.data(), Buffer.data() + Buffer.size());
  Token T;
  // The lexer has no file location, so token locations encode buffer offsets.
  while (!L.LexFromRawLexer(T) || T.isNot(tok::eof)) {
    if (T.is(tok::eof)) break;
    Out.push_back({T.getKind(), Buffer.substr(T.getLocation().getRawEncoding(), T.getLength())});
  }
  return Out;
}

// ---- Rules 20.8 / 20.9: #if expression evaluation ---------------------------
class ConditionEvaluator {
public:
  ConditionEvaluator(Preprocessor &PP, std::vector<RawToken> Tokens) : PP(PP), Toks(std::move(Tokens)) {}
  std::optional<int64_t> run() {
    auto V = conditional(true);
    if (Pos != Toks.size()) return std::nullopt;
    return V;
  }
  std::vector<std::string> Undefined;

private:
  Preprocessor &PP;
  std::vector<RawToken> Toks;
  size_t Pos = 0;
  int Depth = 0;
  bool Failed = false;
  bool at(tok::TokenKind K) const { return Pos < Toks.size() && Toks[Pos].Kind == K; }
  std::optional<int64_t> conditional(bool Eval) {
    auto C = binary(0, Eval);
    if (!at(tok::question)) return C;
    ++Pos;
    bool Take = C && *C != 0;
    auto A = conditional(Eval && C && Take);
    if (!at(tok::colon)) return std::nullopt;
    ++Pos;
    auto B = conditional(Eval && C && !Take);
    if (!C) return std::nullopt;
    return Take ? A : B;
  }
  static int precedence(tok::TokenKind K) {
    switch (K) {
    case tok::pipepipe: return 1;
    case tok::ampamp: return 2;
    case tok::pipe: return 3;
    case tok::caret: return 4;
    case tok::amp: return 5;
    case tok::equalequal: case tok::exclaimequal: return 6;
    case tok::less: case tok::greater: case tok::lessequal: case tok::greaterequal: return 7;
    case tok::lessless: case tok::greatergreater: return 8;
    case tok::plus: case tok::minus: return 9;
    case tok::star: case tok::slash: case tok::percent: return 10;
    default: return -1;
    }
  }
  std::optional<int64_t> binary(int MinPrec, bool Eval) {
    auto L = unary(Eval);
    while (Pos < Toks.size()) {
      tok::TokenKind K = Toks[Pos].Kind;
      int P = precedence(K);
      if (P < 0 || P < MinPrec) break;
      ++Pos;
      bool RightEval = Eval;
      if (K == tok::ampamp && L && *L == 0) RightEval = false;
      if (K == tok::pipepipe && L && *L != 0) RightEval = false;
      auto R = binary(P + 1, RightEval);
      if (!L || !R) {
        if (K == tok::ampamp && L && *L == 0) L = 0;
        else if (K == tok::pipepipe && L && *L != 0) L = 1;
        else L = std::nullopt;
        continue;
      }
      int64_t A = *L, B = *R;
      switch (K) {
      case tok::pipepipe: L = (A || B); break;
      case tok::ampamp: L = (A && B); break;
      case tok::pipe: L = A | B; break;
      case tok::caret: L = A ^ B; break;
      case tok::amp: L = A & B; break;
      case tok::equalequal: L = A == B; break;
      case tok::exclaimequal: L = A != B; break;
      case tok::less: L = A < B; break;
      case tok::greater: L = A > B; break;
      case tok::lessequal: L = A <= B; break;
      case tok::greaterequal: L = A >= B; break;
      case tok::lessless: L = (B >= 0 && B < 63) ? (A << B) : 0; break;
      case tok::greatergreater: L = (B >= 0 && B < 63) ? (A >> B) : 0; break;
      case tok::plus: L = A + B; break;
      case tok::minus: L = A - B; break;
      case tok::star: L = A * B; break;
      case tok::slash: L = B ? std::optional<int64_t>(A / B) : std::nullopt; break;
      case tok::percent: L = B ? std::optional<int64_t>(A % B) : std::nullopt; break;
      default: L = std::nullopt;
      }
    }
    return L;
  }
  std::optional<int64_t> unary(bool Eval) {
    if (Pos >= Toks.size()) return std::nullopt;
    tok::TokenKind K = Toks[Pos].Kind;
    if (K == tok::exclaim || K == tok::tilde || K == tok::minus || K == tok::plus) {
      ++Pos;
      auto V = unary(Eval);
      if (!V) return std::nullopt;
      return K == tok::exclaim ? int64_t(!*V) : K == tok::tilde ? ~*V : K == tok::minus ? -*V : *V;
    }
    return primary(Eval);
  }
  std::optional<int64_t> primary(bool Eval) {
    const RawToken &T = Toks[Pos++];
    if (T.Kind == tok::l_paren) {
      auto V = conditional(Eval);
      if (!at(tok::r_paren)) return std::nullopt;
      ++Pos;
      return V;
    }
    if (T.Kind == tok::numeric_constant) {
      llvm::StringRef Digits = T.Text;
      while (!Digits.empty() && (Digits.back() == 'u' || Digits.back() == 'U' || Digits.back() == 'l' || Digits.back() == 'L'))
        Digits = Digits.drop_back();
      unsigned long long V = 0;
      if (Digits.getAsInteger(0, V)) return std::nullopt;
      return static_cast<int64_t>(V);
    }
    if (T.Kind == tok::raw_identifier || T.Kind == tok::identifier) {
      if (T.Text == "defined") {
        bool Paren = at(tok::l_paren);
        if (Paren) ++Pos;
        if (Pos >= Toks.size()) return std::nullopt;
        bool Defined = PP.isMacroDefined(Toks[Pos++].Text);
        if (Paren) {
          if (!at(tok::r_paren)) return std::nullopt;
          ++Pos;
        }
        return Defined ? 1 : 0;
      }
      const IdentifierInfo *II = PP.getIdentifierInfo(T.Text);
      const MacroInfo *MI = PP.getMacroInfo(II);
      if (!MI) {
        if (Eval) Undefined.push_back(T.Text);
        return 0;
      }
      if (MI->isFunctionLike() || ++Depth > 16) return std::nullopt;
      std::vector<RawToken> Body;
      for (const Token &Tok : MI->tokens()) Body.push_back({Tok.getKind(), PP.getSpelling(Tok)});
      ConditionEvaluator Inner(PP, Body);
      Inner.Depth = Depth;
      auto V = Body.empty() ? std::nullopt : Inner.run();
      if (Eval) Undefined.insert(Undefined.end(), Inner.Undefined.begin(), Inner.Undefined.end());
      return V;
    }
    return std::nullopt;
  }
};
class Conditions final : public PPCallbacks {
public:
  Conditions(Rule &R, Preprocessor &PP, bool Boolean) : R(R), PP(PP), Boolean(Boolean) {}
  void evaluate(SourceLocation Loc, ConditionValueKind Value) {
    if (Value == CVK_NotEvaluated || !user(PP, Loc)) return;
    // Loc is the directive name; the condition follows it on the same line.
    std::string Text = directiveLine(PP.getSourceManager(), Loc);
    if (size_t Comment = Text.find("//"); Comment != std::string::npos) Text.resize(Comment);
    auto Tokens = rawTokens(PP.getLangOpts(), Text);
    if (Tokens.empty()) return;
    Tokens.erase(Tokens.begin());
    ConditionEvaluator E(PP, std::move(Tokens));
    auto V = E.run();
    if (Boolean) {
      if (V && *V != 0 && *V != 1) R.report(Loc, "#if or #elif condition does not evaluate to 0 or 1");
    } else if (!E.Undefined.empty())
      R.report(Loc, "identifier '" + E.Undefined.front() + "' in #if or #elif condition is not defined");
  }
  void If(SourceLocation Loc, SourceRange, ConditionValueKind Value) override { evaluate(Loc, Value); }
  void Elif(SourceLocation Loc, SourceRange, ConditionValueKind Value, SourceLocation) override { evaluate(Loc, Value); }

private:
  Rule &R;
  Preprocessor &PP;
  bool Boolean;
};

// ---- Rule 20.1 / 20.2 / 20.3 ----------------------------------------------------
class Includes final : public PPCallbacks {
public:
  Includes(Rule &R, Preprocessor &PP, int Which) : R(R), PP(PP), Which(Which) {}
  // Offset of the first token that is not part of a preprocessing directive.
  size_t firstCode(FileID Id) {
    auto Found = FirstCode.find(Id);
    if (Found != FirstCode.end()) return Found->second;
    const SourceManager &SM = PP.getSourceManager();
    llvm::StringRef Buffer = SM.getBufferData(Id);
    Lexer L(SM.getLocForStartOfFile(Id), PP.getLangOpts(), Buffer.begin(), Buffer.begin(), Buffer.end());
    Token T;
    bool InDirective = false;
    size_t Result = Buffer.size();
    while (!L.LexFromRawLexer(T)) {
      if (T.isAtStartOfLine()) InDirective = T.is(tok::hash);
      if (!InDirective) {
        Result = SM.getFileOffset(T.getLocation());
        break;
      }
    }
    return FirstCode[Id] = Result;
  }
  void InclusionDirective(SourceLocation Hash, const Token &, StringRef FileName, bool IsAngled, CharSourceRange,
                          OptionalFileEntryRef, StringRef, StringRef, const Module *, bool,
                          SrcMgr::CharacteristicKind) override {
    if (!user(PP, Hash)) return;
    const SourceManager &SM = PP.getSourceManager();
    if (Which == 1) {
      if (SM.getFileOffset(Hash) > firstCode(SM.getFileID(Hash)))
        R.report(Hash, "#include is preceded by code other than preprocessor directives or comments");
    } else if (Which == 2) {
      bool Bad = FileName.contains('\'') || FileName.contains('\\') || FileName.contains("/*") || FileName.contains("//") ||
                 (IsAngled && FileName.contains('"'));
      if (Bad) R.report(Hash, "header name contains a character sequence with undefined behaviour");
    } else {
      std::string Line = directiveLine(SM, Hash);
      if (size_t Comment = Line.find("//"); Comment != std::string::npos) Line.resize(Comment);
      auto Toks = rawTokens(PP.getLangOpts(), Line);
      // Toks: '#', 'include', operand...
      if (Toks.size() < 3) return;
      std::vector<RawToken> Operand(Toks.begin() + 2, Toks.end());
      bool Ok = false;
      if (Operand.front().Kind == tok::string_literal) Ok = Operand.size() == 1;
      else if (Operand.front().Kind == tok::less) Ok = Operand.back().Kind == tok::greater;
      else if (Operand.size() == 1 && Operand.front().Kind == tok::raw_identifier) {
        const MacroInfo *MI = PP.getMacroInfo(PP.getIdentifierInfo(Operand.front().Text));
        Ok = MI && ((MI->getNumTokens() == 1 && MI->getReplacementToken(0).is(tok::string_literal)) ||
                    (MI->getNumTokens() > 1 && MI->getReplacementToken(0).is(tok::less) &&
                     MI->tokens().back().is(tok::greater)));
      }
      if (!Ok) R.report(Hash, "#include is not followed by exactly one <filename> or \"filename\"");
    }
  }

private:
  Rule &R;
  Preprocessor &PP;
  int Which;
  std::map<FileID, size_t> FirstCode;
};

// ---- Rules 20.4 / 20.5 / 20.10 / 20.11 / 20.12 / 20.7 / 20.6 ----------------------
class Macros final : public PPCallbacks {
public:
  Macros(Rule &R, Preprocessor &PP, int Which) : R(R), PP(PP), Which(Which) {}
  void MacroDefined(const Token &Name, const MacroDirective *MD) override {
    const MacroInfo *MI = MD->getMacroInfo();
    if (!user(PP, Name.getLocation())) return;
    auto Toks = MI->tokens();
    if (Which == 4 && Name.getIdentifierInfo()->isKeyword(PP.getLangOpts()))
      R.report(Name.getLocation(), "macro is defined with the name of a keyword");
    if (Which == 10)
      for (const Token &T : Toks)
        if (T.is(tok::hashhash) || (T.is(tok::hash) && MI->isFunctionLike())) {
          R.report(T.getLocation(), "# or ## preprocessor operator used");
          break;
        }
    if (Which == 11 && MI->isFunctionLike())
      for (size_t I = 0; I + 2 < Toks.size(); ++I)
        if (Toks[I].is(tok::hash) && MI->getParameterNum(Toks[I + 1].getIdentifierInfo()) >= 0 && Toks[I + 2].is(tok::hashhash))
          R.report(Toks[I].getLocation(), "macro parameter after # is followed by ##");
    if (Which == 12 && MI->isFunctionLike()) {
      std::set<const IdentifierInfo *> Operand, Plain;
      for (size_t I = 0; I < Toks.size(); ++I) {
        const IdentifierInfo *II = Toks[I].getIdentifierInfo();
        if (!II || MI->getParameterNum(II) < 0) continue;
        bool Op = (I > 0 && Toks[I - 1].isOneOf(tok::hash, tok::hashhash)) || (I + 1 < Toks.size() && Toks[I + 1].is(tok::hashhash));
        (Op ? Operand : Plain).insert(II);
      }
      for (const IdentifierInfo *P : Operand)
        if (Plain.count(P)) Mixed[MI].insert(P);
    }
  }
  void MacroUndefined(const Token &Name, const MacroDefinition &, const MacroDirective *) override {
    if (Which == 5 && user(PP, Name.getLocation())) R.report(Name.getLocation(), "#undef used");
  }
  void directive(SourceLocation Loc) {
    if (Which == 6) Directives.push_back(Loc);
  }
  void If(SourceLocation Loc, SourceRange, ConditionValueKind) override { directive(Loc); }
  void Elif(SourceLocation Loc, SourceRange, ConditionValueKind, SourceLocation) override { directive(Loc); }
  void Ifdef(SourceLocation Loc, const Token &, const MacroDefinition &) override { directive(Loc); }
  void Ifndef(SourceLocation Loc, const Token &, const MacroDefinition &) override { directive(Loc); }
  void Else(SourceLocation Loc, SourceLocation) override { directive(Loc); }
  void Endif(SourceLocation Loc, SourceLocation) override { directive(Loc); }
  void MacroExpands(const Token &Name, const MacroDefinition &MD, SourceRange Range, const MacroArgs *Args) override {
    const MacroInfo *MI = MD.getMacroInfo();
    if (!MI || !user(PP, Name.getLocation())) return;
    const SourceManager &SM = PP.getSourceManager();
    if (Which == 6 && MI->isFunctionLike())
      for (SourceLocation D : Directives)
        if (SM.isPointWithin(D, Range.getBegin(), Range.getEnd()))
          R.report(D, "preprocessing directive appears within macro arguments");
    if (!Args || !MI->isFunctionLike()) return;
    if (Which == 12) {
      auto Found = Mixed.find(MI);
      if (Found == Mixed.end()) return;
      for (const IdentifierInfo *P : Found->second) {
        int Index = MI->getParameterNum(P);
        for (const Token *T = Args->getUnexpArgument(Index); T->isNot(tok::eof); ++T)
          if (T->getIdentifierInfo() && PP.isMacroDefined(T->getIdentifierInfo()))
            R.report(Name.getLocation(), "macro argument is used with and without #/## and is itself a macro");
      }
    }
    if (Which == 7) {
      auto Toks = MI->tokens();
      for (size_t I = 0; I < Toks.size(); ++I) {
        int Index = Toks[I].getIdentifierInfo() ? MI->getParameterNum(Toks[I].getIdentifierInfo()) : -1;
        if (Index < 0) continue;
        bool Operator = (I > 0 && Toks[I - 1].isOneOf(tok::hash, tok::hashhash, tok::period, tok::arrow)) ||
                        (I + 1 < Toks.size() && Toks[I + 1].is(tok::hashhash));
        bool Parenthesized = I > 0 && I + 1 < Toks.size() && Toks[I - 1].is(tok::l_paren) && Toks[I + 1].is(tok::r_paren);
        if (Operator || Parenthesized) continue;
        std::vector<const Token *> Arg;
        for (const Token *T = Args->getUnexpArgument(Index); T->isNot(tok::eof); ++T) Arg.push_back(T);
        if (Arg.size() <= 1) continue;
        if (Arg.front()->is(tok::l_paren) && Arg.back()->is(tok::r_paren)) {
          int Depth = 0;
          bool Whole = true;
          for (size_t K = 0; K < Arg.size(); ++K) {
            Depth += Arg[K]->is(tok::l_paren) ? 1 : Arg[K]->is(tok::r_paren) ? -1 : 0;
            if (Depth == 0 && K + 1 < Arg.size()) Whole = false;
          }
          if (Whole) continue;
        }
        R.report(Name.getLocation(), "expression from macro parameter expansion is not enclosed in parentheses");
        return;
      }
    }
  }

private:
  Rule &R;
  Preprocessor &PP;
  int Which;
  std::vector<SourceLocation> Directives;
  std::map<const MacroInfo *, std::set<const IdentifierInfo *>> Mixed;
};

// ---- Rule 20.13 -------------------------------------------------------------------
class Skipped final : public PPCallbacks {
public:
  Skipped(Rule &R, Preprocessor &PP) : R(R), PP(PP) {}
  void SourceRangeSkipped(SourceRange Range, SourceLocation) override {
    if (!user(PP, Range.getBegin())) return;
    const SourceManager &SM = PP.getSourceManager();
    bool Invalid = false;
    llvm::StringRef Text = Lexer::getSourceText(CharSourceRange::getCharRange(Range), SM, PP.getLangOpts(), &Invalid);
    if (Invalid) return;
    static const std::set<std::string> Valid = {"if", "ifdef", "ifndef", "elif", "else", "endif", "define",
                                                "undef", "include", "line", "error", "pragma", ""};
    bool InComment = false;
    size_t Offset = 0;
    llvm::SmallVector<llvm::StringRef, 64> Lines;
    Text.split(Lines, '\n');
    for (llvm::StringRef Line : Lines) {
      llvm::StringRef Trimmed = Line.ltrim();
      if (!InComment && Trimmed.starts_with("#")) {
        llvm::StringRef Word = Trimmed.drop_front().ltrim();
        Word = Word.take_while([](char C) { return llvm::isAlnum(C) || C == '_'; });
        if (!Valid.count(Word.str()) && Offset > 0)
          R.report(Range.getBegin().getLocWithOffset(Offset + (Line.size() - Trimmed.size())),
                   "line starting with # is not a valid preprocessing directive");
      }
      for (size_t I = 0; I + 1 < Line.size(); ++I) {
        if (!InComment && Line[I] == '/' && Line[I + 1] == '/') break;
        if (!InComment && Line[I] == '/' && Line[I + 1] == '*') InComment = true, ++I;
        else if (InComment && Line[I] == '*' && Line[I + 1] == '/') InComment = false, ++I;
      }
      Offset += Line.size() + 1;
    }
  }

private:
  Rule &R;
  Preprocessor &PP;
};

// ---- Rule 20.14 -------------------------------------------------------------------
class SameFile final : public PPCallbacks {
public:
  SameFile(Rule &R, Preprocessor &PP) : R(R), PP(PP) {}
  void check(SourceLocation Loc, SourceLocation IfLoc) {
    const SourceManager &SM = PP.getSourceManager();
    if (user(PP, Loc) && IfLoc.isValid() && SM.getFileID(SM.getExpansionLoc(Loc)) != SM.getFileID(SM.getExpansionLoc(IfLoc)))
      R.report(Loc, "conditional directive is not in the same file as its #if");
  }
  void Else(SourceLocation Loc, SourceLocation IfLoc) override { check(Loc, IfLoc); }
  void Elif(SourceLocation Loc, SourceRange, ConditionValueKind, SourceLocation IfLoc) override { check(Loc, IfLoc); }
  void Endif(SourceLocation Loc, SourceLocation IfLoc) override { check(Loc, IfLoc); }

private:
  Rule &R;
  Preprocessor &PP;
};

template <typename T, typename... A> Factory pp(A... Args) {
  return [Args...] {
    return ppSpec([Args...](Rule &R, Preprocessor &PP) { return std::make_unique<T>(R, PP, Args...); });
  };
}
} // namespace

void addRulesPreprocessor(Table &T) {
  T.push_back({"misra-c2012-rule-20-1", pp<Includes>(1)});
  T.push_back({"misra-c2012-rule-20-2", pp<Includes>(2)});
  T.push_back({"misra-c2012-rule-20-3", pp<Includes>(3)});
  T.push_back({"misra-c2012-rule-20-4", pp<Macros>(4)});
  T.push_back({"misra-c2012-rule-20-5", pp<Macros>(5)});
  T.push_back({"misra-c2012-rule-20-6", pp<Macros>(6)});
  T.push_back({"misra-c2012-rule-20-7", pp<Macros>(7)});
  T.push_back({"misra-c2012-rule-20-8", pp<Conditions>(true)});
  T.push_back({"misra-c2012-rule-20-9", pp<Conditions>(false)});
  T.push_back({"misra-c2012-rule-20-10", pp<Macros>(10)});
  T.push_back({"misra-c2012-rule-20-11", pp<Macros>(11)});
  T.push_back({"misra-c2012-rule-20-12", pp<Macros>(12)});
  T.push_back({"misra-c2012-rule-20-13", pp<Skipped>()});
  T.push_back({"misra-c2012-rule-20-14", pp<SameFile>()});
}
} // namespace safety::misra
