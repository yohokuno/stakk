#include "../connection.h"
using namespace stakk;

int main(int argc, char *argv[]) {
    string connection_ = "test/connection.txt";
    Connection connection;
    assert( connection.load(connection_) == true );
    assert( connection.size == 4);
    assert( connection.get(0,0) == 0);
    assert( connection.get(1,3) == 5000);
    assert( connection.get(3,3) == 5000);
    return 0;
}
