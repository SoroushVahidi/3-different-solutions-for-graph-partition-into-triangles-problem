#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>
//#include <conio.h>
#include <ctime>

using namespace std;
ifstream fin ;
clock_t tStart;

class graph{
    public:
    long v;
    long m;
    map<long,set<long> >adj;
    set<pair<long,long> >degrees;
    graph(){};
    void print(){
        cout<<v<<endl;
        for(map<long,set<long> >::iterator i=adj.begin();i!=adj.end();i++){
            cout<<i->first<<":  ";
            for(set<long>::iterator  j=i->second.begin();j!=i->second.end();j++)
                cout<<*j<<' ';
            cout<<endl;
        }
        cout<<endl;
    }
    bool is_triangle(long a,long b,long c){
        bool b1=((adj[a].find(b)!=adj[a].end())&(adj[a].find(c)!=adj[a].end()));
        bool b2=(adj[b].find(c)!=adj[b].end());
        return (b1&b2);
    }
     void read(){
      string s;
      char c;
      while(1){
        fin>>c;
        if(c=='c'){
          getline(fin,s);
          //cerr<<s<<endl;
        //  getch();
        }
        if(c=='p'){
          string f;
          fin>>f;
          long e;
          fin>>v>>e;
         // cerr<<v<<' '<<e<<endl;
         // getch();
          degrees.clear();
          for(int i=0;i<adj.size();i++)
            adj[i].clear();

          for(int i=0;i<e;i++){
            fin>>c;
            int p,q;
            fin>>p>>q;
            p--;
            q--;
            adj[p].insert(q);
            adj[q].insert(p);
          }
          for(int i=0;i<v;i++)
            degrees.insert(make_pair(adj[i].size(),i));
          break;
        }
      }
    }
};
/*bool operator<(graph g1,graph g2){
  return (g1.v<g2.v);
}
map<graph,bool> checked;
map<graph,bool> result;*/

graph operator-(graph g,long c){
    graph ans=g;
    g.m-=ans.adj[c].size();
    pair<long,long> f=make_pair(ans.adj[c].size(),c);
    for(set<long>::iterator  i=ans.adj[c].begin();i!=ans.adj[c].end();i++){
        long u=*i;
        long degu=ans.adj[u].size();
        pair<long,long> p=make_pair(degu,u);
        ans.degrees.erase(p);
        if(degu>1)
          ans.degrees.insert(make_pair(degu-1,u));
        ans.adj[u].erase(c);
    }
    ans.adj.erase(c);
    ans.v--;
    ans.degrees.erase(f);
    return ans;
}
class triangle{
    public:
        long a,b,c;
        triangle(long d,long e,long f){
            a=d;
            b=e;
            c=f;
        }
        void print(){
            cout<<a+1<<' '<<b+1<<' '<<c+1<<endl;
        }
};
graph operator-(graph g,triangle c){
    graph ans=g;
    ans=ans-c.a;
    ans=ans-c.b;
    ans=ans-c.c;
    return ans;
}
vector<triangle> ans;
#define op 10000000
  map<long,bool> visited;
  vector<long> tin, low;
  set<pair<long,long> > cutbridge;
  long timer;
map<long,bool> is_checked;
long numvertex(graph g,long f){
  long ans=1;
  is_checked[f]=true;
  for(set<long>::iterator i=g.adj[f].begin();i!=g.adj[f].end();i++)
      if(!is_checked[*i])
        ans+=numvertex(g,*i);
  return ans;
}
bool is_valid(graph g){
  is_checked.clear();
  for(set<pair<long,long> >::iterator it=g.degrees.begin();it!=g.degrees.end();it++){
      long i=it->second;
      if(!is_checked[i]){
          long f=numvertex(g,i);
          if((f%3)!=0){
             // cerr<<i<<' '<<f<<endl;
             g.print();
            return false;
          }
      }
  }
  return true;
}
void dfs(graph g,long v, long p=-1) {
	visited[v]=true;
	tin[v]=low[v]=timer++;
	for(set<long>::iterator to = g.adj[v].begin(); to !=g.adj[v].end(); to++) {
      if(*to==p)
        continue;
    if(visited[*to])
        low[v]=min(low[v],tin[*to]);
    else{
        dfs(g,*to,v);
        low[v] = min(low[v], low[*to]);
        if (low[*to] > tin[v])
          cutbridge.insert(make_pair(v,*to));
			}
		}
	return ;
}
graph operator-(graph g,pair<long,long> x){
    graph ans=g;
   long a=x.first;
   long b=x.second;
   pair<long,long> l=make_pair(ans.adj[a].size(),a);
   ans.adj[a].erase(b);
   ans.degrees.erase(l);
   if(ans.adj[a].size()==0){
      ans.adj.erase(a);
      ans.v--;
   }
   else
      ans.degrees.insert(make_pair(l.first-1,a));
    l=make_pair(ans.adj[b].size(),b);
   ans.adj[b].erase(a);
   ans.degrees.erase(l);
   if(ans.adj[b].size()==0){
      ans.adj.erase(b);
      ans.v--;
   }
   else
      ans.degrees.insert(make_pair(l.first-1,b));
    return ans;
}
graph nocut(graph g){
    graph ans=g;
    visited.clear();
		cutbridge.clear();
		tin.assign(ans.v, -1);
    low.assign(ans.v, -1);
		for(long i = 0; i < g.v; i++)
			if(!visited[i])
          dfs(g,i);
		for(set<pair<long,long> >::iterator i = cutbridge.begin(); i !=cutbridge.end(); i++)
				ans=ans-make_pair(i->first,i->second);
    return ans;
  }
  bool oo=false;
bool solve1(graph g){
      if(oo)
          return false;
      if (((double)(clock() - tStart)/CLOCKS_PER_SEC)>120){
        cout<<"timeout"<<endl;
        oo=true;
        return false;
      }

  /*  cerr<<"new solve"<<endl;
    cerr<<g.degrees.size()<<endl;
    cerr<<g.adj.size()<<endl;
    g.print();
    getch();*/
    if(g.v==0){
      //  cout<<"Solvable!!"<<endl;
       /* for(long i=0;i<ans.size();i++)
            ans[i].print();*/
        return true;
    }
 //   if(g.m<g.v)
   //     return false;
    if((g.v)%3!=0){
       // cout<<"NO Subtance 1"<<endl;
        return false;
    }
  /*  if((g.m<2*g.v)&&(!is_valid(g)))
        return false;*/
    pair<long,long> a=*g.degrees.begin();
   // cerr<<a.second<<' '<<a.first<<endl;
    if(a.first<2)
        return false;
  //  g=nocut(g);
    if(a.first==2){
        long b=*g.adj[a.second].begin();
       set<long,set<long> >:: iterator it=g.adj[a.second].begin();
       it++;
        long c=*it;

        if(!g.is_triangle(a.second,b,c)){
         //   cout<<a.second<<' '<<b<<' '<<c<<endl;

            return false;
        }
        ans.push_back(triangle(a.second,b,c));
       // cerr<<g.v<<endl<<a.second+1<<' '<<b+1<<' '<<c+1<<endl;
        if( solve1(g-triangle(a.second,b,c))){
           // cout<<"solvable"<<endl;
            return true;
        }
        else{
            ans.pop_back();

            return false;
        }
    }

    set<long>::iterator it=g.adj[a.second].begin();
    bool found=false;
    while(it!=g.adj[a.second].end()){
        set<long>::iterator it2=it;
        it2++;
        while(it2!=g.adj[a.second].end()){
            if(g.is_triangle(a.second,*it,*it2)){
                triangle t=triangle(a.second,*it,*it2);
      //          cerr<<g.v<<endl;
        //          cerr<<a.second+1<<' '<<*it+1<<' '<<*it2+1<<endl;
                ans.push_back(t);
                if(solve1((g-t))){
              //    cout<<"solvable"<<endl;
                  return true;
                }
                else{
                 /* if(g.v==573)
                    cerr<<a.second+1<<' '<<*it+1<<' '<<*it2+1<<endl;*/
                  ans.pop_back();
                }
            }
            it2++;
        }
        it++;
        it2=it;
        it2++;
    }

 //   cout<<"NO Substance"<<endl;
    return false;

}

void ptime(){
  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  return;
}
long ted=0;
void cnt(graph g){
    if(g.v==0){
         ted++;
        return ;
    }
    if((g.v)%3!=0){
       // cout<<"NO Subtance 1"<<endl;
        return ;
    }
    pair<long,long> a=*g.degrees.begin();
   // cerr<<a.second<<' '<<a.first<<endl;
    if(a.first<2){
     //   cout<<"NO Subtance 2"<<endl;
        return ;
    }
    if(a.first==2){
        //cerr<<"oomad"<<endl;
        long b=*g.adj[a.second].begin();
       set<long,set<long> >:: iterator it=g.adj[a.second].begin();
       it++;
        long c=*it;
        if(!g.is_triangle(a.second,b,c)){
          //  cout<<"NO Subtance 3"<<endl;
            return;
        }
        ans.push_back(triangle(a.second,b,c));
        cnt(g-triangle(a.second,b,c));
          ans.pop_back();
        return ;

    }
    set<long>::iterator it=g.adj[a.second].begin();
    bool found=false;
    while(it!=g.adj[a.second].end()){
        set<long>::iterator it2=it;
        it2++;
        while(it2!=g.adj[a.second].end()){
            if(g.is_triangle(a.second,*it,*it2)){
                triangle t=triangle(a.second,*it,*it2);
                ans.push_back(t);
                cnt(g-t);
                ans.pop_back();
            }
            it2++;
        }
        it++;
        it2=it;
        it2++;
    }
  //  cout<<"NO Substance"<<endl;
    return ;
}
int main(){
     int tedad=0;

    fin.open("o1.txt");
      graph g;
  //  while(1){
        g.read();
  // g.print();
   // cerr<<"deghat : "<<g.is_triangle(287,286,293)<<endl;
     //   cout<<i<<' ';
      tedad+=  solve1(g);

        fin.close();

  cout<<tedad<<endl;

   // }
    return 0;
}
