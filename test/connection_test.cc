#include "../connection.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    string connection_ = "test/connection.txt";
    Connection connection;
    assert( connection.load(connection_) == true );
    assert( connection.size == 2);
    assert( connection.get(0,0) == 100);
    assert( connection.get(0,1) == 10);
    assert( connection.get(1,0) == 1000);
    assert( connection.get(1,1) == 500);
    return 0;
}
