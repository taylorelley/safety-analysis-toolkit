namespace flight { template<class T>T identity(T x){return x;} int identity(int a,int b){return a+b;} }
int safe(){return flight::identity(1)+flight::identity(1,2);}
