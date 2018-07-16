#include <stdio.h>
#include <string>

#include <toscore/utils/RLP.h>
#include <toscore/common/vector_ref.h>


using std::cout;
using std::string;
dev::bytes rlpEncode();
void rlpDecode(dev::bytes output);
int main(int argc, char const *argv[])
{
    /* code */
    cout<<"encode" << std::endl;
    dev::bytes  byts = rlpEncode();
    //cout<<byts<< std::endl;

    rlpDecode(byts);


    return 0;
}

dev::bytes rlpEncode()
{

    dev::RLPStream root;
    dev::RLPStream stream;

    root.appendList(3);
    stream.appendList(2) << "hello"
                         << "world";
    root.appendRaw(stream.out());

    stream.clear();
    stream.appendList(2) << "xxx"
                         << "chee";
    root.appendRaw(stream.out());

    stream.clear();
    stream.appendList(2) << "ddd"
                         << "dfe";
    root.appendRaw(stream.out());

    const dev::bytes output = root.out();
    string hex = dev::toHex(output);
    printf("encode hex : %s\n", hex.c_str());

    return output;
}

void rlpDecode(dev::bytes output)
{

std::map<int,std::string> banlist= {{1,"hello"},{4,"world"},{3,"lamchee"}};
dev::RLPStream root;
root.append(banlist);


dev::RLP rlp0(root.out());
std::map<int,std::string> ret = rlp0.toMap<int,std::string>();

    for (auto kv : ret)
    {
        printf("key : %d -> value : %s\n",kv.first, kv.second.c_str());
    }
    dev::RLP rlp(output);
    printf("trace1\n");

    //printf("item count : %d\n",(int)rlp.itemCount());
    //bytes header = rlp[0];
    //printf("after decode,start hex : %s,item count : %d\n",toHex(rlp.toByte()).c_str(),(int)rlp.itemCount());

    printf("rlp.isData() == %d, rlp.isList() == %d,rlp.isNull() == %d,rlp.isInt() == %d,rlp.isEmpty() == %d\n",
           rlp.isData(), rlp.isList(), rlp.isNull(), rlp.isInt(), rlp.isEmpty());
    {
        //std::vector<dev::vector_ref.mutable_value_type> vec = rlp.toVector();


string ss = rlp.toStringStrict();

	// template <class T>  std::vector<string> vec = rlp.toVector();
        cout<<"vec:"<<std::endl;
    
	// for( std::vector<string>::iterator it = vec.begin() ;it!=vec.end();it++)
	// {
	// 	cout<<*it<<"     ";
	// }
	cout<<ss;


        string str1 = rlp[0][0].toString();
        string str2 = rlp[2][1].toString();
        printf("str1 : %s, str2 : %s\n", str1.c_str(), str2.c_str());
    }
}
