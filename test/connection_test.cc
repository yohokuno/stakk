#include "../connection.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    try { locale::global(locale("")); } catch (...) {}
    string connection_ = "test/connection.txt";
    Connection connection;
    assert( connection.load(connection_) == true );
    assert( connection.get_size() == 4);
    assert( connection.get(0,0) == 0);
    assert( connection.get(1,3) == 5000);
    assert( connection.get(3,3) == 5000);

    string id_def = "test/id.def";
    Definition definition;
    assert( definition.load(id_def) == true );
    assert( definition.get(1) == L"名詞" );
    return 0;
}
