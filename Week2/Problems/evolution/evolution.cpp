#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <algorithm>
#include <queue>

using namespace std;

class Node
{
	public:
		int age;
		string name;
		Node* parent = NULL;
		vector<Node*> children;
		vector<Node*>* path_till_root;
		int path_start = -1;
		int path_end = -1;

		Node()
		{
		}

		Node(string name)
		{
			this->name = name;
		}


		Node(int age)
		{
			this->age = age;
		}

		bool operator < (Node* n) 
		{
			return this->name < n->name;
		}
};

void print_spec(vector<Node> spec, int n)
{
	for (int i = 0; i < n; ++i)
	{
		cout << endl << spec[i].name << " " << spec[i].age << " Parent=";

		if (spec[i].parent != NULL)
		{
			cout << (spec[i].parent)->name << endl;
		} 
		else
		{
			cout << "NULL" << endl;
		}

		cout << "children:" << endl;
		for (vector<Node*>::iterator it = spec[i].children.begin(); it != spec[i].children.end(); ++it)
		{
			cout << (*it)->name << " ";
		}

		cout << "\nPath till root:" << endl;
		for (int j = spec[i].path_start; j < spec[i].path_end; ++j)
		{
			cout << (*(spec[i].path_till_root))[j]->name << " ";
		}

		cout << endl;
	}
}

// O(qlogn) worst case
void evolution_2()
{
	int n, q; cin >> n >> q;

	vector<Node> spec;
	Node* root;

	for (int i = 0; i < n; ++i)
	{
		Node n;
		cin >> n.name >> n.age;
		spec.push_back(n);

		if (n.name == "luca")
		{
			root = &n;
		}
	}

	// sort the species vector by name, so that searching for the first query node
	// takes O(logn)
	sort(spec.begin(), spec.end(), [] (Node x, Node y) -> bool
										{
											return x.name < y.name;
										}
		);

	// Read in the child-parent relations and build the tree
	for (int i = 0; i < n-1; ++i)
	{
		string child, parent;
		cin >> child >> parent;

		vector<Node>::iterator child_it = lower_bound(spec.begin(), spec.end(), new Node(child) );

		vector<Node>::iterator parent_it = lower_bound(spec.begin(), spec.end(), new Node(parent) );

		child_it->parent = &(*parent_it); // convert iterator to pointer
		(parent_it->children).push_back( &(*child_it) );
	}

	vector<Node*>* vec;

	// Save path till root for each leaf in the tree.
	for (int i = 0; i < n; ++i)
	{
		if ( spec[i].children.empty() )
		{
			vec = new vector<Node*>();

			Node* node = &(spec[i]);

			while(node != NULL)
			{
				node->path_till_root = vec;
				(*vec).push_back(node);
				node = node->parent;
			}
		}
	}

	// Set path_start and path_end for all nodes in the tree.
	for (int i = 0; i < n; ++i)
	{
		if ( spec[i].children.empty() )
		{
			Node* node = &(spec[i]);
			int cur_ind = 0;
			int length = (*(node->path_till_root)).size();

			while(node != NULL)
			{
				node->path_start = cur_ind++;
				node->path_end   = length;

				node = node->parent;
			}
		}
	}

	/*print_spec(spec, n);

	return;*/

	// search for each query in tree.
	for (int i = 0; i < q; ++i)
	{
		string s;
		int b;
		cin >> s >> b;

		vector<Node>::iterator it_query = lower_bound(spec.begin(), spec.end(), new Node(s) );

		// Binary search on path_till_root of the node, depending on age
		Node* node = &(*it_query);

		int l = node->path_start;
		int r = (node->path_end) - 1;

		int mid = l;

		while(l <= r)
		{
			mid = (l+r)/2;

			if ( ((*(node->path_till_root))[mid])->age > b )
			{
				r = mid-1;
			}
			else 
			{
				l = mid+1;
			}
		}

		string sol;
		int length = node->path_end - node->path_start;

		if ( (mid+1 < length) && ((*(node->path_till_root))[mid+1]->age <= b) )
		{
			sol = (*(node->path_till_root))[mid+1]->name;
		}
		else if ((*(node->path_till_root))[mid]->age <= b)
		{
			sol = (*(node->path_till_root))[mid]->name;
		}
		else if (mid-1 >= 0)
		{
			sol = (*(node->path_till_root))[mid-1]->name;
		}

		

		cout << sol << " ";
	}

	cout << endl;
}

// O(nq) if degenarate tree
void evolution_1()
{
	int n, q; cin >> n >> q;

	vector<Node> spec;
	for (int i = 0; i < n; ++i)
	{
		Node n;
		cin >> n.name >> n.age;

		spec.push_back(n);
	}

	// sort the species vector by name, so that searching for the first query node
	// takes O(logn)
	sort(spec.begin(), spec.end(), [] (Node x, Node y) -> bool
										{
											return x.name < y.name;
										}
		);

	// Read in the child-parent relations and build the tree
	for (int i = 0; i < n-1; ++i)
	{
		string child, parent;
		cin >> child >> parent;

		vector<Node>::iterator child_it = lower_bound(spec.begin(), spec.end(), new Node(child) );

		vector<Node>::iterator parent_it = lower_bound(spec.begin(), spec.end(), new Node(parent) );

		child_it->parent = &(*parent_it); // convert iterator to pointer
	}

	//print_spec();

	for (int i = 0; i < q; ++i)
	{
		string s;
		int b;
		cin >> s >> b;

		vector<Node>::iterator it = lower_bound(spec.begin(), spec.end(), new Node(s) );

		Node* child = &(*it);
		Node* parent = child->parent;

		while(parent != NULL)
		{
			if (parent->age > b)
			{
				break;
			}
			else
			{
				child = parent;
				parent = parent->parent;
			}
		}

		cout << child->name << " ";
	}

	cout << endl;
}

// binary search:
// find largest index i s.t. p(i) := age[path[i]] <= b is true
int binary(int b, const vector<int>& path, const vector<int>& age)
{
	// establish invariant
	int l = -1; // l is largest index where we know p(l) is false
	int r = path.size() -1; // r is smallest index where we know p(r) is true
	while(l+1 < r)
	{ 	// always at least one element strictly inbetween
		int m = (l+r)/2; // m!=l and m!=r so interval shrinks
		// propagate invariant
		if(age[path[m]] <= b) r = m;
		else l = m;
	}
	return path[r];
}

// dfs
void dfs(int u, const vector<vector<int> >& tree, vector<int>& path,
	const vector<vector<pair<int,int> > >& query,
	vector<int>& result, const vector<int>& age)
{
	// process queries
	for(int i = 0; i < query[u].size(); ++i)
	{
		result[query[u][i].second] = binary(query[u][i].first,path,age);
	}

	// continue dfs
	for(int i = 0; i < tree[u].size(); ++i)
	{
		int v = tree[u][i];
		path.push_back(v); // maintain path invariant
		dfs(v,tree,path,query,result,age);
		path.pop_back(); // maintain path invariant
	}
}

void evolution_mlogn()
{
	int n, q; std::cin >> n >> q;
	// read names and ages
	unordered_map<std::string,int> species_to_index;
	vector<std::string> species(n);
	vector<int> age(n);
	for(int i = 0; i < n; ++i) 
	{
		string name; cin >> name;
		species_to_index[name] = i;
		species[i] = name;
		cin >> age[i];
	}

	// find root
	int root = max_element(age.begin(), age.end()) - age.begin();
	
	// read tree
	vector<vector<int> > tree(n);
	for(int i = 0; i < n-1; ++i)
	{
		string child; cin >> child;
		string parent; cin >> parent;
		tree[species_to_index[parent]].push_back(species_to_index[child]);
	}

	// read queries;
	// for each species store a vector of queries consisting of
	// the age b and the index of the query i
	vector<vector<pair<int,int> > > query(n);
	for(int i = 0; i < q; ++i)
	{
		string name; cin >> name;
		int b; cin >> b;
		query[species_to_index[name]].push_back(make_pair(b,i));
	}

	// process queries in one tree traversal
	vector<int> path; path.push_back(root); // init path invariant
	vector<int> result(q);
	dfs(root,tree,path,query,result,age);

	// output result
	for(int i = 0; i < q; ++i)
	{
		cout << species[result[i]];
		if(i < q-1) cout << " ";
	}
	cout << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);

	int t; cin >> t;
	for (int i = 0; i < t; ++i)
	{
		evolution_mlogn();
	}
	
	return 0;
}