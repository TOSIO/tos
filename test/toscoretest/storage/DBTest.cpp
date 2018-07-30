#include <iostream>
#include "leveldb/db.h"
using namespace std;
using namespace leveldb;


int main(int argc, char *argv[])
{
    DB *db;
    Options options;
    options.create_if_missing = true;
    const char *db_path = argv[1];
    Status status = DB::Open(options, db_path, &db);
    if(!status.ok())
    {
        cout<<"Failde to open levedb:"<<db_path<<endl;
    }
    else
    {
        cout<<"Open successfully!"<<endl;
    }
    string key1 = "Num1";
    string val1 = "Val1";
    Status s = db->Put(WriteOptions(), key1, val1);
    assert(s.ok());
    /*
    WriteOptions write;
    write.sync = true;
    Status s = db->Put(write, key1, val1);
    */
    string key = key1;
    string val = val1;
    s = db->Get(ReadOptions(), key, &val1);
    cout<<"Get value:"<<endl;
    assert(s.ok());
    s = db->Delete(WriteOptions(), key);
    assert(s.ok());
    delete db;
    return 0;
}