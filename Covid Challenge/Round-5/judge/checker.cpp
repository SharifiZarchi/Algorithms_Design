#include <bits/stdc++.h>
using namespace std;

const int N=501;
const int MAX_DAYS=(int)1e6;
const int MAX_OPS=(int)1e6;

int n,m;
int p[N],pos[N];
int e[N][N];

int cnt;
bool ok[N][N];

int k;
int cur_day=1;
int dids_in_day[N];
long long day_cost;

long long res;

int seen;
bool mark[N];
bool dfs(int x)
{
	mark[x]=1;
	seen++;
	for(int i=1;i<=n;i++)
		if(i!=x && e[i][x]!=-1 && !mark[i])
			dfs(i);
	return seen==n;
}

long long checker(string input_dest,string output_dest)
{
	ifstream testin(input_dest);
	ifstream resin(output_dest);
	testin>>n>>m;
	if(n<2 || m<n-1 || m>n*(n-1)/2)return cerr<<"Bad input format!(n,m)\n",-1;
	for(int i=1;i<=n;i++)
	{
		testin>>p[i];
		if(p[i]<0 || p[i]>(int)1e9)return cerr<<"Bad input format!(p[i])\n",-1;
		pos[i]=i;
		day_cost+=p[i];
	}
	memset(e,-1,sizeof e);
	for(int i=1;i<=m;i++)
	{
		int u,v,c;
		testin>>u>>v>>c;
		if(u<1 || u>n || v<1 || v>n || u==v || e[u][v]!=-1)return cerr<<"Bad input format!(u,v)\n",-1;
		if(c<0 || c>(int)1e9)return cerr<<"Bad input format!(c[i])\n",-1;
		e[u][v]=e[v][u]=c;
	}
	if(!dfs(1))return cerr<<"Country not connected!\n",-1;
	resin>>k;
	if(k>MAX_OPS)return cerr<<"Too much operations!\n",-1;
	for(int i=1;i<=k;i++)
	{
		int t,d,x,y;
		if(!(resin>>t>>d>>x>>y))return cerr<<"Op"<<i<<": Bad output format\n",-1;
		if(x<1 || x>n || y<1 || y>n)return cerr<<"Op"<<i<<": Numbers are in wrong format\n",-1;
		if(d>MAX_DAYS || d<cur_day)return cerr<<"Op"<<i<<": Days are in wrong format\n",-1;
		if(d>cur_day)
		{
			memset(dids_in_day,0,sizeof dids_in_day);
			res+=day_cost*(d-cur_day);
			cur_day=d;
		}
		if(t==1)
		{
			if(e[pos[x]][y]==-1)return cerr<<"Op"<<i<<": Edge doesn't exist!(q=1)\n",-1;
			if(dids_in_day[x]>=1)return cerr<<"Op"<<i<<": Wrong opertion format!(q=1)\n",-1;
			dids_in_day[x]=1;
			res+=e[pos[x]][y];
			day_cost-=p[pos[x]];
			day_cost+=p[y];
			pos[x]=y;
		}
		else if(t==2)
		{
			if(x==y || pos[x]!=y || pos[y]!=y || dids_in_day[x]>=2 || dids_in_day[y]>=2 || ok[x][y])return cerr<<"Op"<<i<<": Wrong operation format!(q=2)\n",-1;
			ok[x][y]=true;
			dids_in_day[x]=dids_in_day[y]=2;
			cnt++;
		}
		else return cerr<<"Op"<<i<<": Bad output format(t)\n",-1;
	}
	res+=day_cost;
	if(cnt!=n*(n-1))return cerr<<"All visits aren't done\n",-1;
	return res;
}

int main(int argc, char** argv)
{
	cout<<checker(argv[1], argv[2]) << endl;;
	return 0;
}
