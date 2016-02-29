#include <unistd.h>

void cunlink(){
  unlink("./target.txt");
}

int main() {
  cunlink();
}
