#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>
#include <ctime>
using namespace std;
clock_t tStart;
ifstream fin ("o7.txt");
void ptime(){
  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  return;
}
map<pair<int,int>,long long >combination;
long long cnr(long long n,long long r){
    if(combination[make_pair(n,r)])
        return combination[make_pair(n,r)];
      long long g=n;
      long long h=r;
    long long p = 1, k = 1;

    // C(n, r) == C(n, n-r),
    // choosing the smaller value
    if (n - r < r)
        r = n - r;

    if (r != 0) {
        while (r) {
            p *= n;
            k *= r;
            long long m = __gcd(p, k);

            // dividing by gcd, to simplify product
            // division by their gcd saves from the overflow
            p /= m;
            k /= m;

            n--;
            r--;
        }
    }
    else
        p = 1;
    combination[make_pair(g,h)]=p;
    return p;
}
class graph{
    public:
    int v;
    map<int,set<int> >adj;
    set<pair<int,int> >degrees;
   /* graph(int n,map<int,set<int> >a){
        v=n;
        adj=a;
        degrees.clear();
        for(int i=0;i<v;i++)
            degrees.insert(make_pair(adj[i].size(),i));
    }*/
    graph(){};
    void print(){
        cout<<v<<endl;
        for(map<int,set<int> >::iterator i=adj.begin();i!=adj.end();i++){
            cout<<i->first<<":  ";
            for(set<int>::iterator  j=i->second.begin();j!=i->second.end();j++)
                cout<<*j<<' ';
            cout<<endl;
        }
        cout<<endl;
    }
    bool is_triangle(int a,int b,int c){
        bool b1=((adj[a].find(b)!=adj[a].end())&(adj[a].find(c)!=adj[a].end()));
        bool b2=(adj[b].find(c)!=adj[b].end());
        return (b1&b2);
    }
    void read(){
        while(1){
            int m;
           // string h;
            //stringstream s;
            //getline(fin,h);
            //cerr<<h<<endl;
            //s<<h;
         /*   char c;
            fin>>c;
            fin>>h;*/
            fin>>v>>m;

            /*s>>c;
            if(c=='c')
                continue;*/
           // if(c=='p'){
              degrees.clear();
              for(int i=0;i<v;i++)
                adj[i].clear();
             // s>>h;
             // s>>v;
             // s>>m;
              for(int i=0;i<m;i++){
                char z;
                int v1,v2;
                fin>>v1>>v2;
               // cerr<<v1<<' '<<v2<<endl;
                adj[v1].insert(v2);
                adj[v2].insert(v1);
              }
              for(int i=0;i<v;i++)
                degrees.insert(make_pair(i,adj[i].size()));
            //}
            break;
        }
    }
};

/*graph operator-(graph g,int c){
    graph ans=g;
    for(set<int>::iterator  i=ans.adj[c].begin();i!=ans.adj[c].end();i++){
        int u=*i;
        int degu=ans.adj[u].size();
        pair<int,int> p=make_pair(u,degu);
        ans.degrees.erase(p);
        ans.degrees.insert(make_pair(u,degu-1));
        ans.adj[u].erase(c);
    }
    ans.adj.erase(c);
    ans.v--;
    return ans;
}*/
class triangle{
    public:
        int a,b,c;
        triangle(int d,int e,int f){
            a=d;
            b=e;
            c=f;
        }
        void print(){
            cout<<a<<' '<<b<<' '<<c<<endl;
        }
};
/*graph operator-(graph g,triangle c){
    graph ans=g;
    ans=ans-c.a;
    ans=ans-c.b;
    ans=ans-c.c;
    return ans;
}*/
vector<triangle> tri;
void findt(graph g){
  int v=g.v;
  tri.clear();
  for(int i=0;i<v;i++){
    set<int>::iterator j=g.adj[i].upper_bound(i);
    set<int>::iterator l=j;
    l++;
    while(j!=g.adj[i].end()){
      while(l!=g.adj[i].end()){
        if((g.is_triangle(i,*j,*l))&&(i<*j)&&(*j<*l))
            tri.push_back(triangle(i,*j,*l));
        l++;
      }
      j++;
      l=j;
      l++;
    }
  }
 // for(int i=0;i<tri.size();i++)
   // tri[i].print();
  return;
}
bool noncover(vector<triangle> f){
  if(f.size()<2)
    return true;
  set<int> ver;
  for(int i=0;i<f.size();i++){
      ver.insert(f[i].a);
      ver.insert(f[i].b);
      ver.insert(f[i].c);
  }
  return ((ver.size())==(3*f.size()));
}

bool ok(triangle a,triangle b){
    set<int> ra;
    ra.insert(a.a);
    ra.insert(a.b);
    ra.insert(a.c);
    ra.insert(b.a);
    ra.insert(b.b);
    ra.insert(b.c);
    return (ra.size()==6);
}



long ie(graph g){
  findt(g);
  long si=tri.size();
  long ans=0;
  vector<triangle> r;
  long long i=0;
  for(i=0;i<(1<<si);){
    long long j=i;
    r.clear();
    bool br=false;
    for(int h=0;h<si;h++){
      if(j%2){
          for(int l=0;l<r.size();l++)
            if(!ok(tri[h],r[l])){
                i+=(1<<l);
                br=true;
                break;
            }
          r.push_back(tri[h]);
      }
      if(br)
          break;
      j/=2;
    /*  if(!noncover(r)){
        br=true;
        i++;
        break;
      }*/
    }
    if(br)
        continue;
      long long sum=0;
    long b=g.v-3*r.size();
    for(long h=0;h<=b;h++){
     //   cerr<<b<<' '<<h<<' '<<cnr(b,h)<<endl;
        if(h%2)
          sum-=cnr(b,h);
        else
          sum+=cnr(b,h);
    }
    ans+=sum;
    i++;
   // cerr<<sum<<endl;
  }
  return ans;
}

int main(){
    tStart=clock();
   /* while(1){
      long long n,r;
      cin>>n>>r;
      cerr<<cnr(n,r)<<endl;
    }*/
   // ptime();
  // for(int i=0;i<5;i++){
    graph g;
   // ptime();

  //  while(1){
    g.read();
   // ptime();
    //findt(g);
      cerr<<ie(g)<<endl;
 // }
  ptime();
    //}
    return 0;
}
