#include "Block.h"


namespace dev
{
namespace sdag
{
class MainChain {
    public:
    
    void check();
    void addBlock(Block &b);



    private:
    Block *pre_main_chain;
    void confirmTx();





}
}
}