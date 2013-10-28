#include<iostream>
#include<stack>

class Tower {
  std::stack<int> disks;
  std::string name;
public:
  Tower(const std::string& nm) : name(nm) {};
  const std::string& Name() const { return name; }
  int Size() const { return disks.size(); }
  int Top() const { return disks.top(); }
  int Pop() {
    if(disks.empty()) {
      std::cerr << "ERROR: popping from empty stack" << std::endl;
      return -1;
    }
    const int res = disks.top();
    disks.pop();
    return res;
  }
  int Push(int d) {
    if(!disks.empty() && disks.top() <= d) {
      std::cerr << "ERROR: disk " << d << " is too large: top element is " << disks.top() << std::endl;
    } else {
      disks.push(d);
    }
  }
};

void MoveOne( Tower &from, Tower &to ) {
  std::cout << from.Name() << " -> " << to.Name() << " : disk " 
	    << from.Top() << std::endl;
  to.Push( from.Pop() );
}

// Given a stack at &beg, move N elements to &end
void Move( int N, Tower &beg, Tower &mid , Tower &end ) {
  if(N==1) {
    MoveOne(beg,end);
    return;
  }
  // Move N-1 disks to middle
  Move(N-1,beg,end,mid);
  // move bottom disk to final location
  MoveOne(beg,end);
  // Move the rest from middle to final
  Move(N-1,mid,beg,end);
}

int main() {
  Tower start("1");
  for(int i=2; i>=0; i--) start.Push(i);
  Tower middle("2");
  Tower end("3");
  Move(start.Size(),start,middle,end);
}

