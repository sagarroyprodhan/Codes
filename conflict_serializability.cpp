#include<iostream>
#include<fstream>
#include<list>
#include<cstring>
#include<stack>

using namespace std;


class Graph{
	int V;
	list<int> *adj;
	bool isCyclicUtil(int v, bool visited[], bool *rs);  // used by isCyclic()
	void topologicalSortUtil(int v, bool visited[], stack<int> &mystack);

public:
	Graph(int v);
	void add_edge(int s,int d);
	void print_adjacency_list();
	bool isCyclic();    // returns true if there is a cycle in this graph
	void topologicalSort();

};

Graph::Graph(int v){
	this->V = v;
	adj = new list<int>[V+1];
}

void Graph::add_edge(int s,int d){
	adj[s].push_back(d);

}

void Graph::print_adjacency_list(){
	for(int i=1;i<V+1;i++){
		cout<<i<<"-->";
		list<int>::iterator it;
		for(it=adj[i].begin(); it!=adj[i].end(); it++){
			cout<<*it<<",";
		}
		cout<<endl;
	}
}

// This function is a variation of DFSUytil() in http://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack)
{
    if(visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;
 
        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            if ( !visited[*i] && isCyclicUtil(*i, visited, recStack) )
                return true;
            else if (recStack[*i])
                return true;
        }
 
    }
    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}
 
// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in http://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclic()
{
    // Mark all the vertices as not visited and not part of recursion
    // stack
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 1; i < V+1; i++)
    {
        visited[i] = false;
        recStack[i] = false;
    }
 
    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(int i = 1; i < V+1; i++)
        if (isCyclicUtil(i, visited, recStack))
            return true;
 
    return false;
}

void Graph::topologicalSortUtil(int v,bool visited[],stack<int> &mystack){

	visited[v]=true;

	for(list<int>::iterator it=adj[v].begin(); it!=adj[v].end() ; it++){
		if(!visited[*it])
			topologicalSortUtil(*it,visited,mystack);
	}

	mystack.push(v);

}

void Graph::topologicalSort(){

	stack<int> mystack;
	bool *visited = new bool[V+1];

	for(int i=1;i<V+1;i++)
		visited[i]=false;

	for(int i=1;i<V+1;i++)
		if(!visited[i])
			topologicalSortUtil(i,visited,mystack);

	while(!mystack.empty()){
		cout<<mystack.top()<<" ";
		mystack.pop();
	}

}


int main(int argc,char *argv[]){

	char *inputfile = argv[1];

	char operation[10];
	int tid,val,count=0,n=0;

	FILE *fp;
	fp = fopen(inputfile,"r");

	do{
		
		if(fgetc(fp)=='c')count++;

	}while(!feof(fp));

	fseek(fp,0,SEEK_SET);

	while(!feof(fp)){
		
		if(fgetc(fp)=='\n')n++;

	}	

	Graph g(count);

	int a[n+1][3],i=0;

	fseek(fp,0,SEEK_SET);

	do{

			fscanf(fp,"%s %d %d",operation,&tid,&val);
			if(!strcmp(operation,"commit")){
				//cout<<operation<<" "<<tid<<endl;
				a[i][0]=2;a[i][1]=tid;
				i++;												//commit op is 2
			}
			else if(!strcmp(operation,"R")){
				//cout<<operation<<" "<<tid<<" "<<val<<endl;
				a[i][0]=0;a[i][1]=tid;a[i][2]=val;
				i++;												//read op is 0
			}
			else if(!strcmp(operation,"W")){
				//cout<<operation<<" "<<tid<<" "<<val<<endl;
				a[i][0]=1;a[i][1]=tid;a[i][2]=val;
				i++;												//write op is 1
			}

			

	}while(!feof(fp));

	fclose(fp);

	for(int i=0;i<n+1;i++){
		if(a[i][0]==2)continue;
		for(int j=i+1;j<n+1;j++){
			if(a[i][0]==1 && a[j][0]==1 && a[i][1]!=a[j][1] && a[i][2] == a[j][2]){
				g.add_edge(a[i][1],a[j][1]);
				//cout<<a[i][1]<<" "<<a[j][1]<<endl;
			}
			else if((a[i][0]== 1- a[j][0]) && a[i][1]!=a[j][1] && a[i][2] == a[j][2]){
				g.add_edge(a[i][1],a[j][1]);
				//cout<<a[i][1]<<" "<<a[j][1]<<endl;
			}
		}
	}

	g.print_adjacency_list();
	cout<<endl;

	if(!g.isCyclic()){
		cout<<"A Serial Schedule for this would be:"<<endl;
		g.topologicalSort();
	}
	else
		cout<<"The Schedule is NOT Conflict Serializable"<<endl;

	return 0;
}