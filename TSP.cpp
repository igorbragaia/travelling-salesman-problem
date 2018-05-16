#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> pp;
#define INF 2<<30


struct pq_element{
	int weight, vertex, parent;
	pq_element(int wweight, int vvertex, int pparent): weight(wweight), vertex(vvertex), parent(pparent) {};
	bool operator>(const pq_element & x) const {
		return weight > x.weight;
	}
};


class Solution{
public:
	Solution(vector<pp>&vs, int&v){
		total_vertices = v;
		vector<pp>vertices = vs;
		create_mst(0);
	}

	int cost(){
		int answer = 0;
		for(map<pp,int>::iterator it = edges.begin(); it != edges.end(); it++)
			answer += it->second;
		return answer;
	}

	void print_mst(){
		cout << "Minimum spanning tree:" << endl;
		for(map<pp,int>::iterator it = edges.begin(); it != edges.end(); it++)
				cout << it->first.first << " - " << it->first.second << endl;
		cout << endl << "MST cost: " << cost() << endl;
	}
private:
	int total_vertices;
	vector<pp>vertices;
	map<pp,int>edges;

	void create_mst(int start){
		vector<int>key(total_vertices+1,INF);
		vector<int>parent(total_vertices+1,-1);
		vector<bool>inMST(total_vertices+1,false);
		priority_queue<pq_element,vector<pq_element>,greater<pq_element>> pq;

		key[start] = 0;
		pq_element el(key[start], start, -1);
		pq.push(el);

		while(!pq.empty()){
			int weight = pq.top().weight;

			vector<pq_element> elements;
			while(weight == pq.top().weight){
				pq_element front = pq.top();
				pq.pop();

				if(!inMST[front.vertex]){
					elements.push_back(front);
				}
			}

			// sort(elements.begin(), elements.end(), compare(neighbors));

			if((int)elements.size() > 0 ){
				pq_element front = elements[0];
				for(int i = 1; i < (int)elements.size(); i++)
					pq.push(elements[i]);

				inMST[front.vertex] = true;
				parent[front.vertex] = front.parent;

				for(int i = 1; i <= total_vertices; i++)
					if(i != front.vertex){
						int distance = dij(vertices[i], vertices[front.vertex]);
						if(!inMST[i] and key[i] >= distance){
							key[i] = distance;
							pq_element el(key[i], i, front.vertex);
							pq.push(el);
						}
					}
			}
		}

		for(int i = 1; i <= total_vertices; i++)
			if(parent[i] != -1)
				edges[make_pair(min(i,parent[i]),max(i,parent[i]))] = dij(vertices[i], vertices[parent[i]]);
	}

	static int dij(pp i, pp j){
		int xd = i.first - j.first, yd = i.second - j.second;
		int x = sqrt(xd*xd + yd*yd);
		return (int)(x + 0.5);
	}
};



int main(){
	int n;
	cin >> n;

	ofstream writingfile;
	writingfile.open("saida.txt");

	/* file variables */
	ifstream readingfile;
	string filename;
	/* file variables */

	/* vertices variables */
	int total_vertices, vertice, x, y;
	vector<pp> vertices;
	/* vertices variables */

	for(int i = 1; i <= n; i++){
		filename = "ent"  + (i<10)?"0":"" + to_string(i) + ".txt";
		readingfile.open(filename);
		readingfile >> total_vertices;

		vertices.clear();
		vertices.resize(total_vertices + 1);

		for(int i = 1; i <= total_vertices; i++){
			readingfile >> vertice >> x >> y;
			vertices[vertice] = make_pair(x,y);
		}
		readingfile.close();

		Solution * solve = new Solution(vertices,total_vertices);
		// writingfile << solve.answer << endl;
		delete solve;
	}

	writingfile.close();

	return 0;
}
