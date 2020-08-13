#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>
#include <termios.h>
#include <unistd.h>

//#include <conio.h>

using namespace std;

ofstream fout("sat.txt");
ifstream pin ("satout.txt");
ofstream kout("graph.txt");
ofstream lout("s.txt");
string ppp;

int getch(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

int kind_of_adj(int q[],int siz){
  if(siz==4){
    sort(q,q+siz);
    if((q[0]==0)&&(q[1]==0)&&(q[2]==0)&&(q[3]==0))
      return 0;
    if((q[0]==0)&&(q[1]==0)&&(q[2]==1)&&(q[3]==1))
      return 1;
    if((q[0]==0)&&(q[1]==1)&&(q[2]==1)&&(q[3]==2))
      return 2;
    if((q[0]==1)&&(q[1]==1)&&(q[2]==1)&&(q[3]==1))
      return 3;
    if((q[0]==1)&&(q[1]==1)&&(q[2]==1)&&(q[3]==3))
      return 4;
    if((q[0]==1)&&(q[1]==1)&&(q[2]==2)&&(q[3]==2))
      return 5;
    if((q[0]==0)&&(q[1]==2)&&(q[2]==2)&&(q[3]==2))
      return 6;
    if((q[0]==1)&&(q[1]==2)&&(q[2]==2)&&(q[3]==3))
      return 7;
    if((q[0]==2)&&(q[1]==2)&&(q[2]==2)&&(q[3]==2))
      return 8;
    if((q[0]==2)&&(q[1]==2)&&(q[2]==3)&&(q[3]==3))
      return 9;
    if((q[0]==3)&&(q[1]==3)&&(q[2]==3)&&(q[3]==3))
      return 10;
  }
  return -1;
}
ifstream fin ;
string sat_or_not;
set<int> ans_of_sat;
class cluase{
  public:
  int a,b,c;
  cluase(int d,int e,int f){
    a=d;
    b=e;
    c=f;
  }
};
bool operator<(cluase a,cluase b){
  if(a.a!=b.a)
    return (a.a<b.a);
  if(a.b!=b.b)
    return (a.b<b.b);
  return (a.c<b.c);
}
int num=1;
class graph{
    public:
    int v;
    map<int,set<int> >adj;
    set<pair<int,int> >degrees;
    map<int,int> c;
    map<int,int> ham;
    graph(int n,map<int,set<int> >a){
      v=n;
      adj=a;
      ham.clear();
      c.clear();
      degrees.clear();
      for(int i=0;i<v;i++)
        degrees.insert(make_pair(adj[i].size(),i));
    }
    graph(){
    c.clear();
    ham.clear();
    };
    void print(){
      cerr<<adj.size()<<endl;
      for(map<int,set<int> >::iterator i=adj.begin();i!=adj.end();i++){
        cerr<<i->first<<":  ";
        for(set<int>::iterator  j=i->second.begin();j!=i->second.end();j++)
          cerr<<*j<<' ';
        cerr<<"ham=  "<<ham[i->first]<<' ';
        if(is_lit(i->first))
            cerr<<"literal barabar ba  "<<c[i->first];
        cerr<<endl;
      }
      cerr<<endl;
      return;
    }
    bool is_triangle(int a,int b,int c){
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
    int hamsa(int f){
      int q[4];
        set<int>::iterator it=this->adj[f].begin();
        int ja=0;
        while(it!=this->adj[f].end()){
          q[ja]=*it;
          ja++;
          it++;
        }
        bool mat[4][4]={{false}};
        for(int i=0;i<4;i++)
          for(int j=i+1;j<4;j++)
              mat[i][j]=mat[j][i]=(this->adj[q[i]].find(q[j])!=this->adj[q[i]].end());
        //  cerr<<l<<endl;
        bool boreshi=false;
        vector<pair<int,int> > darajeha;
        darajeha.clear();
        for(int i=0;i<4;i++){
          int p=0;
          for(int j=0;j<4;j++)
            p+=mat[i][j];
          darajeha.push_back(make_pair(p,q[i]));
        }
        sort(darajeha.begin(),darajeha.end());
        int seq[4];
        for(int i=0;i<4;i++)
          seq[i]=darajeha[i].first;
          int typ=kind_of_adj(seq,4);
        return typ;
    }
    bool is_lit(int f){
      bool ans=false;
      set<int>::iterator it=this->adj[f].begin();
      while(it!=this->adj[f].end()){
        if(hamsa(*it)==4){
          ans=true;
          break;
        }
        it++;
      }
      return ans;
    }
};

graph operator-(graph g,int c){
    graph ans=g;
    pair<int,int> f=make_pair(ans.adj[c].size(),c);
    for(set<int>::iterator  i=ans.adj[c].begin();i!=ans.adj[c].end();i++){
        int u=*i;
        int degu=ans.adj[u].size();
        pair<int,int> p=make_pair(degu,u);

 //       if(degu>1)
          ans.degrees.insert(make_pair(degu-1,u));
        ans.adj[u].erase(c);
        ans.degrees.erase(p);


    }
    ans.adj.erase(c);
    ans.v--;
    ans.degrees.erase(f);
    return ans;
}
class triangle{
    public:
      int a,b,c;
      triangle(int d,int e,int f){
        a=d;
        b=e;
        c=f;
      }
      void print(){
        lout<<a<<' '<<b<<' '<<c<<endl;
      }
};
graph operator-(graph g,triangle c){
    graph ans=g;
    ans=ans-c.a;
    ans=ans-c.b;
    ans=ans-c.c;
    return ans;
}
vector<triangle> solution;
graph operator-(graph g,pair<int,int> x){
//cerr<<"hell"<<endl;
    graph ans=g;
   int a=x.first;
   int b=x.second;
   pair<int,int> l=make_pair(ans.adj[a].size(),a);
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
int common(set<int> a,set<int> b){
  set<int>::iterator it=a.begin();
  int ted=0;
  while(it!=a.end()){
    ted+=(b.find(*it)!=b.end());
    it++;
  }
  return ted;
}
bool equall(int a[],int b[],int siz){
  for(int i=0;i<siz;i++)
      if(a[i]!=b[i])
        return false;
    return true;
}

vector<int> fehrest;
int bfs(graph g,int a){
  map<int,bool> checked;
  checked.clear();
  checked[a]=true;
  fehrest.clear();
  fehrest.push_back(a);
  for(int i=0;i<fehrest.size();i++){
    set<int>::iterator it=g.adj[fehrest[i]].begin();
    while(it!=g.adj[fehrest[i]].end()){
      if(!checked[*it]){
        fehrest.push_back(*it);
        checked[*it]=true;
      }
      it++;
    }
  }
  return fehrest.size();
}
bool bfs(graph& g,int shom,int takhsis){
  if(g.c[shom]!=0)
    return false;
 // cerr<<"salam"<<endl;
  set<int>::iterator it;
  vector<int> saf;
  saf.clear();
  saf.push_back(shom);
  g.c[shom]=takhsis;
  int i=0;
  while(i<saf.size()){
    it=g.adj[saf[i]].begin();
    while(it!=g.adj[saf[i]].end()){
      if((g.c[*it]==0)&&(g.hamsa(*it)==3)){
        if(g.is_lit(*it))
          g.c[*it]=g.c[saf[i]];
        else
          g.c[*it]=-g.c[saf[i]];
        saf.push_back(*it);
      }
      it++;
    }
    i++;
  }
  return true;
}

bool tosat(graph& g){
  set<cluase> ans;
  ans.clear();
  map<int,set<int> >::iterator it=g.adj.begin();
  while(it!=g.adj.end()){
      //cerr<<it->first<<endl;

      if((g.hamsa(it->first)==3)&&(g.is_lit(it->first))){
       // cerr<<num<<' '<<mosbat<<' '<<manfi<<endl;
        if((bfs(g,it->first,num)))
          num++;
      }
      it++;
  }
  it=g.adj.begin();
  set<int>::iterator alter;
  while(it!=g.adj.end()){
    if(g.hamsa(it->first)==4){
      vector<int> qw;
      qw.clear();
      set<int>::iterator l=g.adj[it->first].begin();
      while(l!=g.adj[it->first].end()){
        if(g.hamsa(*l)!=4)
          qw.push_back(*l);
        l++;
      }
      ans.insert(cluase(g.c[qw[0]],g.c[qw[1]],g.c[qw[2]]));
    }
    it++;
  }
  set<cluase> normalsat;
  normalsat.clear();
  set<cluase>::iterator p=ans.begin();
  int i=1;
  while(p!=ans.end()){
      normalsat.insert(*p);
      normalsat.insert(cluase(-(p->a),-(p->b),0));
      normalsat.insert(cluase(-(p->b),-(p->c),0));
      normalsat.insert(cluase(-(p->a),-(p->c),0));

      p++;
  }
  p=normalsat.begin();
  fout<<"p cnf "<<num-1<<' '<<normalsat.size()<<endl;
  while(p!=normalsat.end()){
    fout<<p->a<<' '<<p->b;
    if((p->c)!=0)
      fout<<' '<<p->c;
    fout<<" 0"<<endl;
    i++;
    p++;
  }
  system("minisat /home/soroush/Desktop/project1/finaltests/sat.txt  /home/soroush/Desktop/project1/finaltests/satout.txt");
  pin>>sat_or_not;
  if(sat_or_not=="UNSAT"){
    cout<<"No solution for triangle partitioning"<<endl;
    return false;
  }
  ans_of_sat.clear();
  int y;
  while(pin>>y){
    if(y==0)
        break;
  //  cerr<<y<<endl;
    ans_of_sat.insert(y);
  }
  it=g.adj.begin();
  while(it!=g.adj.end()){
    if(g.hamsa(it->first)==4){
      vector<int> qw;
      qw.clear();
      qw.push_back(it->first);
      set<int>::iterator l=g.adj[it->first].begin();
      while(l!=g.adj[it->first].end()){
        if((g.hamsa(*l)==4)||(ans_of_sat.find(g.c[*l])!=ans_of_sat.end()))
          qw.push_back(*l);
   //       cerr<<*l<<' '<<g.hamsa(*l)<<endl;
        l++;
      }
      triangle tp(qw[0],qw[1],qw[2]);
      solution.push_back(tp);
      g=g-tp;
      it=g.adj.begin();
    //  break;
     // g.print();
   //  cerr<<g.adj.size()<<endl;
   //   getch();
//      cin>>ppp;

    }
    else
        it++;


  }
 // g.print();
 // return g;
 // cerr<<num<<endl;
  return true;
}
bool solve1(graph g){
  //  g.print();
 //   getch();
    if(g.v==0){
        cout<<"solvable!!"<<endl;
        for(int i=0;i<solution.size();i++)
            solution[i].print();
        return true;
    }
    if((g.v)%3!=0){
       // cout<<"NO Subtance1"<<endl;
        return false;
    }
    pair<int,int> a=*g.degrees.begin();
    if(a.first<2){
     //   cout<<"NO Subtance2"<<endl;
        return false;
    }
    if(a.first==2){
        int b=*g.adj[a.second].begin();
       set<int,set<int> >:: iterator it=g.adj[a.second].begin();
       it++;
        int c=*it;
        if(!g.is_triangle(a.second,b,c)){
           // cout<<"NO Subtance3"<<' '<<a.second<<endl;
            return false;
        }
        solution.push_back(triangle(a.second,b,c));
        g=g-triangle(a.second,b,c);
        return solve1(g);
    }
    if(a.first==3){
        int q[3];
        set<int>::iterator it=g.adj[a.second].begin();
        int ja=0;
        while(it!=g.adj[a.second].end()){
          q[ja]=*it;
          ja++;
          it++;
        }
        int l=0;
         for(int i=0;i<3;i++)
          for(int j=i+1;j<3;j++)
            l+=(g.adj[q[i]].find(q[j])!=g.adj[q[i]].end());
            if(l<1){
           //   cout<<"NO Subtance4"<<endl;
              return false;
            }
        for(int i=0;i<3;i++)
          for(int j=i+1;j<3;j++)
            if(g.is_triangle(a.second,q[i],q[j])){
                solution.push_back(triangle(a.second,q[i],q[j]));
                bool ok=solve1(g-triangle(a.second,q[i],q[j]));
                if(ok)
                  return ok;
                else
                  solution.pop_back();
            }
         // cout<<"NO SOLUTION"<<endl;
          return false;
    }
    if(a.first==4){
      set<pair<int,int> >::iterator f=g.degrees.begin();
      int z=bfs(g,a.second);
      if(z==6){
            set<int>::iterator it=g.adj[a.second].begin();
            set<int>::iterator it2=it;
            it2++;
            while(it!=g.adj[a.second].end()){
              while(it2!=g.adj[a.second].end()){
                  if(g.is_triangle(a.second,*it,*it2)){
                    solution.push_back(triangle(a.second,*it,*it2));
                    g=g-triangle(a.second,*it,*it2);
                    return solve1(g);
                  }
                  it2++;
              }
              it++;
              it2=it;
              it2++;
            }
        }
     tekrar:
        if(f==g.degrees.end()){

          //  g.print();
           return tosat(g);

        }
        int q[4];
        set<int>::iterator it=g.adj[a.second].begin();
        int ja=0;
        while(it!=g.adj[a.second].end()){
          q[ja]=*it;
          ja++;
          it++;
        }
        bool mat[4][4]={{false}};
        for(int i=0;i<4;i++)
          for(int j=i+1;j<4;j++)
              mat[i][j]=mat[j][i]=(g.adj[q[i]].find(q[j])!=g.adj[q[i]].end());
        //  cerr<<l<<endl;
        bool boreshi=false;
        vector<pair<int,int> > darajeha;
        darajeha.clear();
        for(int i=0;i<4;i++){
          int p=0;
          for(int j=0;j<4;j++)
            p+=mat[i][j];
          darajeha.push_back(make_pair(p,q[i]));
          if(p==0){
              boreshi=true;
              g=g-make_pair(a.second,q[i]);
          }
        }
        if(boreshi)
            return solve1(g);
        sort(darajeha.begin(),darajeha.end());
        int seq[4];
        for(int i=0;i<4;i++)
          seq[i]=darajeha[i].first;
          int typ=kind_of_adj(seq,4);
          g.ham[a.second]=typ;
    /*      cerr<<"typ = "<<typ<<endl;
          cerr<<"a.second=  "<<a.second<<endl;
          cout<<"moonde!"<<endl<<endl;*/
        if((typ==0)||(typ==9)||(typ==10)){
          //cout<<"NO substance"<<endl;
          return false;
        }
        if((typ==3)||(typ==4)){
          f++;
          if(f!=g.degrees.end())
            a=*f;
          goto tekrar;
        }
        if(typ==7){//casa 4a
          g=g-make_pair(darajeha[1].second,darajeha[3].second);
          g=g-make_pair(darajeha[2].second,darajeha[3].second);
          return solve1(g);
        }
       // cerr<<a.second<<' '<<a.first<<endl;
        //cerr<<seq[0]<<' '<<seq[1]<<' '<<seq[2]<<' '<<seq[3]<<endl;
        //cerr<<u[0]<<' '<<u[1]<<' '<<u[2]<<' '<<u[3]<<endl;
        //cerr<<darajeha[0].second<<' '<<darajeha[1].second<<' '<<darajeha[2].second<<' '<<darajeha[3].second<<endl;
        if(typ==8){
          for(int i=0;i<4;i++)
            for(int j=i+1;j<4;j++)
              if(mat[i][j]){
                int o=5;
                int y=5;
                for(int x=0;x<4;x++)
                  if((x!=i)&&(x!=j))
                if(o==5)
                  o=x;
                else
                  y=x;
                   // cerr<<q[i]<<' '<<q[j]<<' '<<q[o]<<' '<<q[y]<<endl;
                if((common(g.adj[q[i]],g.adj[q[j]])==1)&&(common(g.adj[q[o]],g.adj[q[y]])==1))
                  g=g-make_pair(q[i],q[j]);
              }
              return solve1(g);
          }
          if(typ==5){
            if(z%3){
          //    cout<<"No substance"<<endl;
              return false;
            }
             set<int>::iterator it=g.adj[a.second].begin();
            set<int>::iterator it2=it;
            it2++;
            while(it!=g.adj[a.second].end()){
              while(it2!=g.adj[a.second].end()){
                  if(g.is_triangle(a.second,*it,*it2)){
                    solution.push_back(triangle(a.second,*it,*it2));

                    if( solve1(g-triangle(a.second,*it,*it2)))
                        return true;
                    else
                        solution.pop_back();
                  }
                  it2++;
              }
              it++;
              it2=it;
              it2++;
            }
          }
    }
    return false;
}
int main(){
 // Minisat::Solver solver;
 // auto A = solver.newVar();
 int tedad=0;
  fin.open("test1.txt");
  graph g;
  g.read();
 /* while(1){
  int ra,rb,rc;
  cin>>ra>>rb>>rc;
  g=g-triangle(ra,rb,rc);
  cerr<<g.adj.size()<<endl;
  }*/
 // g.print();
 // g=g-0;
 //// g.print();
 // g=g-1;
 // g=g-2;
 // g.print();
 // g=g-3;
 // g=g-218;
 // g.print();

  tedad+=solve1(g);
  cout<<tedad<<endl;
  fin.close();
    //}
    return 0;
}
