#include<iostream>
#include<functional>
#include<algorithm>
#include<vector>
#include<list>
#include<queue>
#include<stack>
#include<map>
#include<iterator>
#include<utility>
#include<boost/bind.hpp>
#include<boost/assign/list_of.hpp>
#include<cstdlib>
#include<cassert>
#include<bitset>

template<typename T>
class IsOdd : public std::unary_function<T,bool> {
public:
  bool operator() (const T& t) const { return t%2!=0; }
};

template<typename T>
void dump( const T& a , std::string memo="" ){
  //void dump( const std::vector<int>& a , std::string memo="" ){
  if(memo!="") std::cout << memo << " : ";
  std::copy(a.begin(),a.end(),std::ostream_iterator<int>(std::cout," "));
  std::cout << std::endl;
}

template<typename T>
class OffBy20 {
public:
  bool operator() (const T& x, const T& y) const {
    return y == x+20;
  }
};

template<typename T>
struct MultiplyOffset : public std::binary_function<int,int,int> {
  T operator() (const T& x, const T& y, const T& offset ) const { return x*y + offset; }
};

class Random {
public:
  Random(int seed=0) { std::srand(seed); }
  int operator() () const { return std::rand()%100; }
};

int main() {
  const int data[] = {1,2,3,4,5,6,7};
  const int ndata = sizeof(data)/sizeof(data[0]);

  std::vector<int> a;

  std::cout << "Odd elements" << std::endl;
  std::remove_copy_if(data,data+ndata,std::back_inserter(a),std::not1(IsOdd<int>()));
  dump(a);


  std::cout << "Plus 1" << std::endl;
  std::transform(a.begin(),a.end(), &a[0], std::bind1st(std::plus<int>(),1));
  dump(a);

  std::cout << "Negate" << std::endl;
  std::transform(a.begin(),a.end(), &a[0], std::negate<int>());
  dump(a);

  std::cout << "Abs" << std::endl;
  std::transform(a.begin(),a.end(),&a[0],std::ptr_fun<int,int>(std::abs));
  dump(a);

  std::cout << "Reverse-sorted" << std::endl;
  std::sort(a.begin(),a.end(),std::not2(std::less<int>()));
  dump(a);

  std::cout << "Times 10, then sorted" << std::endl;
  std::transform(a.begin(),a.end(),&a[0],std::bind2nd(std::multiplies<int>(),10));
  std::stable_sort(a.begin(),a.end(),std::less<int>());
  dump(a);

  const int needle[] = {30,40};
  std::vector<int>::iterator it = std::find_first_of(a.begin(),a.end(),needle,needle+2);
  if(it!=a.end()) std::cout << "Found first_of at: " << std::distance(a.begin(),it) << std::endl;

  it = a.begin(); std::advance(it,2); std::cout << "Advanced to: " << *it << std::endl;
  a.insert(it,60);
  dump(a);
  it = std::adjacent_find(a.begin(),a.end(),std::equal_to<int>());
  if(it!=a.end()) std::cout << "Adjacent find returned: " << *it << " at " << std::distance(a.begin(),it) << std::endl;
  it = std::adjacent_find(a.begin(),a.end(),OffBy20<int>());
  if(it!=a.end()) std::cout << "OffBy20 find returned: " << *it << " at " << std::distance(a.begin(),it) << std::endl;

  a.insert(a.begin()+3,13);
  dump(a);
  std::cout << "Number of sixties: " << std::count(a.begin(),a.end(),60) << std::endl;
  std::cout << "Number odd: " << std::count_if(a.begin(),a.end(),IsOdd<int>()) << std::endl;
  dump(a);

  const int chk1[] = {20,40,60,13,60,80};
  std::cout << "Checking correct/incorrect equal: "
	    << std::equal(a.begin(),a.end(),chk1) << std::endl;
  const int chk2[] = {20,40,60,13,70,80};
  {
    std::pair<std::vector<int>::iterator,const int*> itp = std::mismatch(a.begin(),a.end(),chk2);
    if(itp.first!=a.end()) {
      std::cout << "Mismatch at: " << *itp.first << " at " << std::distance(a.begin(),itp.first) << std::endl;
    }
  }

  a.insert(a.begin()+1,40);
  a.insert(a.begin()+1,40);
  dump(a);
  it = std::search_n(a.begin(),a.end(),2,40);
  if(it!=a.end()) std::cout << *it << " at " << std::distance(a.begin(),it) << std::endl;

  std::cout << "midway status report:" << std::endl;
  dump(a);
  std::transform(a.begin(),a.end(),a.begin(), std::bind2nd(std::divides<int>(),5));
  dump(a);
  std::cout << "Putting 10s" << std::endl;
  std::fill(a.begin(),a.end(),10);
  dump(a);
  std::cout << "re-filling from the array" << std::endl;
  std::copy(data,data+ndata,a.begin());
  dump(a);
  std::cout << "iterator swap" << std::endl;
  std::iter_swap(a.begin(), a.begin()+2);
  std::copy(a.begin(),a.end(),std::ostream_iterator<int>(std::cerr," ")); std::cerr << std::endl;

  std::cout << "applying a*a+100" << std::endl;
  std::transform(a.begin(),a.end(),a.begin(),std::back_inserter(a),boost::bind(MultiplyOffset<int>(),_1,_2,100));
  dump(a);
  std::cout << "replacing 200 with 155" << std::endl;
  std::replace(a.begin(),a.end(),200,155);
  dump(a);
  std::cout << "replacing all elements <= 101 with 50" << std::endl;
  std::replace_if(a.begin(),a.end(),std::bind2nd(std::greater_equal<int>(),101),50);
  dump(a);

  std::cout << "Generating random values" << std::endl;
  std::generate(a.begin(),a.end(),Random(10));
  dump(a);

  std::cout << "removing 73" << std::endl;
  it = std::remove(a.begin(),a.end(),73); a.resize( std::distance(a.begin(),it) );
  dump(a);
  std::cout << "removing  elements greater than 70" << std::endl;
  it = std::remove_if(a.begin(),a.end(),std::bind2nd(std::greater<int>(),70)); a.resize( std::distance(a.begin(),it) );
  dump(a);

  std::cout << "adding duplicate 18 and then removing it" << std::endl;
  a.insert(a.begin()+2,18);
  dump(a);
  it = std::unique(a.begin(),a.end()); a.resize(std::distance(a.begin(),it));
  dump(a);
  std::cout << "reverse:" << std::endl;
  std::reverse(a.begin(),a.end());
  dump(a);
  std::cout << "random:" << std::endl;
  std::random_shuffle(a.begin(),a.end());
  dump(a);
  std::cout << "rotate around middle" << std::endl;
  std::rotate(a.begin(), a.begin() + a.size()/2, a.end());
  dump(a);

  std::cout << "partition those less than 10" << std::endl;
  std::partition(a.begin(),a.end(),std::bind2nd(std::less<int>(),10));
  dump(a);

  std::cout << "partial sort first four" << std::endl;
  std::partial_sort(a.begin(),a.begin()+4,a.end());
  dump(a);

  std::cout << "min/max: "
	    << *std::min_element(a.begin(),a.end())
	    << " "
	    << *std::max_element(a.begin(),a.end())
	    << std::endl;

  dump(a);
  std::cout << "nth element(4) vs sorted" << std::endl;
  std::nth_element(a.begin(), a.begin()+4, a.end());
  dump(a);
  std::sort(a.begin(),a.end());
  dump(a);

  a = boost::assign::list_of(3)(4)(5)(6)(7);
  dump(a);
  it = std::lower_bound(a.begin(),a.end(),4);
  assert(*it == 4);
  it = std::upper_bound(a.begin(),a.end(),4);
  assert(*it == 5);

  {
    std::cout << "merge" << std::endl;
    a = boost::assign::list_of(2)(4)(6);
    std::vector<int> b = boost::assign::list_of(3)(5)(6)(7);
    std::vector<int> out;
    std::merge(a.begin(),a.end(),b.begin(),b.end(),std::back_inserter(out));
    dump(a); dump(b); dump(out);
  }

  {
    std::cout << "permutations" << std::endl;
    a = boost::assign::list_of(1)(2)(3);
    int i=0;
    do {
      std::cout << ++i << " : "; dump(a);
    } while (std::next_permutation(a.begin(),a.end()));
  }
  
  std::cout << "Playing with the heap" << std::endl;
  a = boost::assign::list_of(5)(2)(4)(1)(8)(3);
  dump(a,"original");
  std::make_heap(a.begin(),a.end());
  dump(a,"heapified");
  std::cout << "front element: " << a.front() << std::endl;
  a.push_back(9); std::push_heap(a.begin(),a.end());
  dump(a,"re-heapify after adding 9");
  std::pop_heap(a.begin(),a.end()); a.pop_back();
  dump(a,"popped 9");
  std::sort_heap(a.begin(),a.end());
  dump(a,"sorted");

  {
    std::cout << "bitset" << std::endl;
    std::bitset<4> b;
    std::cout << b << std::endl;
    b.set(3); b.flip(1);
    std::cout << b << std::endl;
    b.reset();
    std::cout << b << std::endl;
  }

  {
    std::cout << "list" << std::endl;
    std::list<int> b = boost::assign::list_of(5)(3)(7)(1);
    std::list<int> c = boost::assign::list_of(10)(20);
    dump(b,"list b"); dump(c,"list c");
    std::list<int>::iterator it = b.begin();
    std::advance(it,1);
    b.splice(it,c,c.begin(),c.end());
    dump(b,"spliced c into a");
    b.sort();
    dump(b,"sorted");
  }

  {
    std::cout << "multimap" << std::endl;
    std::multimap<int,char> b;
    b.insert( std::make_pair<int,char>(1,'a') );
    b.insert( std::make_pair<int,char>(2,'a') );
    b.insert( std::make_pair<int,char>(2,'c') );
    b.insert( std::make_pair<int,char>(1,'d') );
    for(std::map<int,char>::const_iterator it=b.begin(); it!=b.end(); it++) {
      std::cout << it->first << " : " << it->second << std::endl;
    }
    std::pair<std::map<int,char>::iterator,std::map<int,char>::iterator> it2 = b.equal_range(2);
    while(it2.first != it2.second) {
      std::cout << "2s: " << it2.first->first << " - " << it2.first->second << std::endl;
      it2.first++;
    }
  }

  //queue, priority queue, stack
  {
    std::cout << "queue" << std::endl;
    std::queue<int> b;
    b.push(4); b.push(2); b.push(3);
    while( !b.empty() ) {
      std::cout << b.front() << std::endl;
      b.pop();
    }
  }
  {
    std::cout << "stack" << std::endl;
    std::stack<int> b;
    b.push(4); b.push(2); b.push(3);
    while(!b.empty()) {
      std::cout << b.top() << std::endl;
      b.pop();
    }
  }
  {
    std::cout << "priority queue" << std::endl;
    std::priority_queue<int,std::vector<int>,std::less<int> > bmax;
    std::priority_queue<int,std::deque<int>,std::greater<int> > bmin;
    bmax.push(5); bmax.push(3); bmax.push(7);
    bmin.push(5); bmin.push(3); bmin.push(7);
    std::cout << "max = " << bmax.top() << " vs " << bmin.top() << std::endl;
  }
}
