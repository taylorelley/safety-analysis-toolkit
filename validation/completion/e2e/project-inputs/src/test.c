int decide(int a, int b);
/** @test T-DEC
 * @req R-DEC */
int main(void) { return decide(0,0) + decide(1,1) == 1 ? 0 : 1; }
