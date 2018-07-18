#include <stdio.h>
#include <string>
#include <list>
#include <toscore/utils/RLP.h>
#include <toscore/common/vector_ref.h>
#include <toscore/common/Exceptions.h>

#define BOOST_TEST_MODULE maintest
 #include <boost/test/included/unit_test.hpp>
//#include <boost/test/unit_test.hpp>


#include <boost/smart_ptr.hpp>
using namespace boost;
using namespace boost::unit_test;
using namespace dev;
using std::cout;
using std::string;
dev::bytes rlpEncode();
void rlpDecode(dev::bytes output);
// int main(int argc, char const *argv[])
// {
//     /* code */
//     cout<<"encode" << std::endl;
//     dev::bytes  byts = rlpEncode();
//     //cout<<byts<< std::endl;

//     rlpDecode(byts);


//     return 0;
// }

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

// std::map<int,std::string> banlist= {{1,"hello"},{4,"world"},{3,"lamchee"}};
// dev::RLPStream root;
// root.append(banlist);


// dev::RLP rlp0(root.out());
// std::map<int,std::string> ret = rlp0.toMap<int,std::string>();

    // for (auto kv : ret)
    // {
    //     printf("key : %d -> value : %s\n",kv.first, kv.second.c_str());
    // }
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

void runRlpTest()
{

    BOOST_CHECK_MESSAGE(2 + 2 == 4, "description…");
    try
    {

        BOOST_CHECK_MESSAGE(2 + 2 == 4, "description…");
        cout << "encode" << std::endl;
        dev::bytes byts = rlpEncode();
        //cout<<byts<< std::endl;

        rlpDecode(byts);
    }
    catch (Exception const &_e)
    {
        BOOST_ERROR("Failed test with Exception: " << diagnostic_information(_e));
    }
    catch (std::exception const &_e)
    {
        BOOST_ERROR("Failed test with Exception: " << _e.what());
    }
}

BOOST_AUTO_TEST_SUITE(s_smart_ptr)
 

BOOST_AUTO_TEST_CASE(runRlpTest)
{
BOOST_CHECK_MESSAGE(2+2==4, "description…" );
    runRlpTest();
}


BOOST_AUTO_TEST_CASE(t_scoped_ptr)
{
    scoped_ptr<int> p(new int(874));
    BOOST_CHECK(p);
    BOOST_CHECK_EQUAL(*p, 874);
 
    p.reset();
    BOOST_CHECK(p == 0);
}
 
BOOST_AUTO_TEST_CASE(t_shared_ptr)
{
    shared_ptr<int> p(new int(100));
 
    BOOST_CHECK(p);
    BOOST_CHECK_EQUAL(*p, 100);
    BOOST_CHECK_EQUAL(p.use_count(), 1);
    shared_ptr<int> p2 = p;
    BOOST_CHECK_EQUAL(p, p2);
    BOOST_CHECK_EQUAL(p2.use_count(), 2);
 
    *p2 = 255;
    BOOST_CHECK_EQUAL(*p, 255);
    BOOST_CHECK_GT(*p, 200);
}

BOOST_AUTO_TEST_SUITE_END()