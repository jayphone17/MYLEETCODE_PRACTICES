#include <bits/stdc++.h>
using namespace std;
#define inf 1145141919

int n=0, e=0;
unordered_map<int, int> hashmap;			// �������ֵ��±��ӳ�� 
unordered_map<int, int> hashmap_rev;
vector<vector<int>> adj;					// �ڽӱ� 
vector<vector<int>> adj_rev;				// ���ڽӱ� 
vector<vector<int>> dis;					// dis[x][i]��ʾ��x���㵽��i����ѯ�ʻ����̾��� 
vector<unordered_set<int>> words;			// words[x]��ʾx�ڵ�����дʻ�ļ��� 
vector<int> vis;							// ���ʿ������� 
vector<unordered_map<int, int>> word_dis;	// x�㵽���дʻ�ľ���map 
vector<int> indegree;						// ��� 

void load_data(string edge_set_path, string key_set_path)
{
	string filepath = edge_set_path;
	list<string> lines;
	cout<<"Reading data and mapping..."<<endl;
	
	ifstream ifs(filepath);				// ���ļ��� 
	streambuf *ori_in = cin.rdbuf();	//����ԭ�������������ʽ
	cin.rdbuf(ifs.rdbuf());				// ���ض��� 
	while(getline(cin,filepath)) 		// ��ȡ�ļ���
	{
		if(filepath.length()==0) break;
		lines.push_back(filepath);
	} 
	ifs.close();						// �ر��� 
	cin.rdbuf(ori_in);					//�ص�����̨�ı�׼�������
	unordered_set<int> hash;			
	for(auto it=lines.begin(); it!=lines.end(); it++)
	{
		int v1=0, v2=0, i=0, len=(*it).length();
		while((*it)[i]!=':') v1*=10,v1+=((*it)[i++]-'0');
		i += 2;
		hash.insert(v1); 
		while(i<len)
		{
			v2=0;
			while((*it)[i]!=',') v2*=10,v2+=((*it)[i++]-'0');
			hash.insert(v2);
			i++;
		}
	}
	for(auto it=hash.begin(); it!=hash.end(); it++)
	{
		hashmap[*it] = n++;
		hashmap_rev[n-1] = *it;
	}
	// ����size��ʼ�� 
	adj.resize(n); 
	adj_rev.resize(n);
	dis.resize(n);
	words.resize(n);
	vis.resize(n);
	word_dis.resize(n);
	indegree.resize(n);
	cout<<"Constructing Adjacency list..."<<endl;
	for(auto it=lines.begin(); it!=lines.end(); it++)
	{
		int v1=0, v2=0, i=0, len=(*it).length();
		while((*it)[i]!=':') v1*=10,v1+=((*it)[i++]-'0');
		i += 2;
		while(i<len)
		{
			v2=0;
			while((*it)[i]!=',') v2*=10,v2+=((*it)[i++]-'0');
			if(hashmap.find(v2)!=hashmap.end())
			{
				adj[hashmap[v1]].push_back(hashmap[v2]);
				adj_rev[hashmap[v2]].push_back(hashmap[v1]);
			}	
			indegree[hashmap[v2]]++;
			i++;
		}
	}
	cout<<"Loading vertices' vocabularies..."<<endl;
	filepath = key_set_path;
	ifstream ifs1(filepath);				
	streambuf *ori_in1 = cin.rdbuf();	
	cin.rdbuf(ifs1.rdbuf());				
	while(getline(cin,filepath)) 		
	{
		if(filepath.length()==0) break;
		int v1=0, v2=0, i=0, len=filepath.length();
		while(filepath[i]!=':') v1*=10,v1+=(filepath[i++]-'0');
		i += 2;
		while(i<len)
		{
			v2=0;
			while(filepath[i]!=',') v2*=10,v2+=(filepath[i++]-'0');
			words[hashmap[v1]].insert(v2);
			i++;
		}
	} 
	cout<<"Loading Completed..."<<endl;
	cout<<"There are "<<n<<" Vertices"<<endl;
	ifs1.close();					
	cin.rdbuf(ori_in1);		
}
bool is_control(vector<int>& m1, vector<int>& m2)
{
	int cnt1=0, cnt2=0;
	for(int i=0; i<m1.size()-1; i++)
	{
		if(m1[i] < m2[i]) cnt1++;
		if(m1[i] <= m2[i]) cnt2++;
	}
	return (cnt1>0)&&(cnt2==m1.size()-1);
}

bool is_inside(int x, vector<int>& query_words)
{
	for(int i=0; i<query_words.size(); i++)
		if(words[x].find(query_words[i])!=words[x].end()) return true;
	return false;
}
void bfs_all(int x)
{
	queue<int> q;
	q.push(x); vis[x]=x;
	int step = 0;
	
	while(!q.empty())
	{
		int qs = q.size();
		for(int sq=0; sq<qs; sq++)
		{
			// ȡ��ǰԪ�� 
			int tp=q.front(); q.pop();
			vis[tp] = x;
			
			// �鿴���������дʻ� ���Ը��¾��� 
			for(auto it=words[tp].begin(); it!=words[tp].end(); it++)
				if(word_dis[x].find(*it)==word_dis[x].end())
					word_dis[x].insert(pair<int,int>(*it, step));
			// bfs 
			for(int i=0; i<adj[tp].size(); i++)
				if(vis[adj[tp][i]]!=x) q.push(adj[tp][i]), vis[adj[tp][i]]=x;
		}
		step++;
	}
}
void load_worddis()
{
	for(int i=0; i<n; i++) vis[i]=-1;
	for(int i=0; i<n; i++) 
	{
		if(i%1000==0) cout<<"Already loaded "<< i << "Words..." << endl;
		bfs_all(i);	
	}
}
void query_in_preprocess(vector<int>& query_words)
{
	vector<vector<int>> glocs;
	for(int i=0; i<n; i++)
	{
		vector<int> res(query_words.size()); res.push_back(i);
		int j;
		for(j=0; j<query_words.size(); j++)
		{
			auto it = word_dis[i].find(query_words[j]);
			if(it==word_dis[i].end()) break;
			else res[j]=it->second;
		}
		if(j<query_words.size()) continue;
		for(j=0; j<glocs.size(); j++)
		{
		 	if(is_control(glocs[j], res)) break;
		 	if(is_control(res, glocs[j])) glocs[j]=res;
		}
		if(j==glocs.size()) glocs.push_back(res);
	}
	for(int i=0; i<n; i++) vis[i]=-1;
	int cnt=0;
	for(int i=0; i<glocs.size(); i++)
	{
		if(vis[glocs[i].back()]==glocs[i].back()) continue;
		vis[glocs[i].back()]=glocs[i].back();
		cnt++;
		for(int j=0; j<glocs[i].size()-1; j++)
		{
			cout<<"<"<<query_words[j]<<", "<<glocs[i][j]<<">  ";
		}cout<<"The root node is :  " << glocs[i].back()<<endl;
	}
	cout<<"Result : "<<cnt<<" nodes"<<endl;
}

int main()
{
	clock_t st, ed;
	double t1=0, t2=0, t3=0, t4=0, t5=0, t6=0;
	load_data("datas/Yago_small/edge.txt", "datas/Yago_small/node_keywords.txt");
	// load_data("datas/Yago_big/edge.txt", "datas/Yago_big/node_keywords.txt");
	cout<<"Pre-Processing..."<<endl; 
	load_worddis();	// Ԥ���� ֻ������small���� 
	// vector<int> query_words{8632116, 9484081,8694912,10565862};
	vector<int> query_words{11674756,11381939,10701562};
	st = clock();
	query_in_preprocess(query_words);
	ed = clock();
	cout<<"time used : "<<(double)(ed-st)/CLOCKS_PER_SEC<< " s" <<endl;
	return 0;
}
