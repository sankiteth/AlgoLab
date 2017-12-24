#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor VertexDescriptor;
const VertexDescriptor NULL_VERTEX = boost::graph_traits<Graph>::null_vertex();
typedef std::vector<std::string> Interests;

// A linear traversal over the two sorted lists of interests.
// Returns true if there are at least ‘k’ common interests.
bool haveKMatches(const Interests& s1, const Interests& s2, int k) 
{
	int i1 = 0, i2 = 0;
	// Stop if reaching the end of one of the lists
	// or there are already enough common interests.
	while(i1 < s1.size() && i2 < s2.size() && k > 0) 
	{
		if(s1[i1] == s2[i2])
		--k;
		// Move the pointer to the ‘smaller’ interest.
		// In case of equality both pointers are moved on after another.
		if(s1[i1] <= s2[i2])
			++i1;
		else
			++i2;
	}

	return k <= 0;
}

void testcases() 
{
	int n, c, f;
	std::cin >> n >> c >> f;
	// Construct a list of sorted interests for every buddy.
	std::vector<Interests> students(n);
	for(int i = 0; i < n; ++i) 
	{
		students[i].resize(c);
		for(int j = 0; j < c; ++j)
			std::cin >> students[i][j];
		std::sort(students[i].begin(), students[i].end());
	}

	// Construct a graph with edges between similar enough buddies.
	Graph G(n);
	for(int i = 0; i < n; ++i)
		for(int j = i + 1; j < n; ++j)
			if(haveKMatches(students[i], students[j], f + 1))
				add_edge(i, j, G);

	// Do maximum matching.
	std::vector<VertexDescriptor> mate(n);
	boost::edmonds_maximum_cardinality_matching(G, &mate[0]);

	// Check if all buddies are included in the matching.

	bool success = true;
	for(int i = 0; i < n; ++i)
		if(mate[i] == NULL_VERTEX) 
		{
			success = false;
			break;
		}

	std::cout << (success ? "not optimal" : "optimal") << '\n';
}

int main() 
{
	std::ios_base::sync_with_stdio(false);
	int T;
	std::cin >> T;
	for (; T > 0; --T)
		testcases();
	
	return 0;
}