#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<algorithm>

// simple hash table

// stored value; int
// hash: string   hash function: string -> int [0..100]
struct HashMap {
  typedef std::list<int> Bucket;
  std::vector< Bucket > buckets;
  HashMap() {
    buckets.resize(100);
  }
  int hash(const std::string& s) {
    int sum=0;
    for(int i=0; i<s.size();i++) sum+=std::abs((i+1)*int(s[i]));
    return std::abs(sum)%100;
  }
  void insert(const std::string& key, const int& value) {
    int bucket = hash(key);
    buckets[bucket].push_back(value);
  }
  void dump() {
    std::vector<Bucket>::const_iterator ib = buckets.begin();
    for( ; ib!= buckets.end(); ib++) {
      const Bucket& buck = *ib;
      for( Bucket::const_iterator it = buck.begin(); it!=buck.end(); it++) {
	std::cout << *it << std::endl;
      }
    }
  }
};

int main() {
 HashMap m;
 m.insert("hello",1);
 m.insert("bye",15);
 m.dump();
}
