#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> pp;
#define INF 2<<29


struct pq_element{
    int weight, vertex, parent;
    pq_element(int wweight, int vvertex, int pparent): weight(wweight), vertex(vvertex), parent(pparent) {};
    bool operator>(const pq_element & x) const {
        return weight > x.weight;
    }
};


struct compare {
    compare(int minMSTEl) { 
        minMSTElement = minMSTEl;
    }
    bool operator () (pq_element x, pq_element y) { 
        if(x.parent == minMSTElement and y.parent != minMSTElement)
            return true;
        if(x.parent != minMSTElement and y.parent == minMSTElement)
            return false;
        return x.vertex < y.vertex;
    }
    int minMSTElement;
};

class Solution{
public:
    Solution(vector<pp>&vs, unsigned int&v){
        total_vertices = v;
        vertices = vs;
        int start = 1;
        create_mst(start);
        create_cycle(start);
    }

    int cost(){
        int answer = 0;
        for(pair<pp,int> el:cycle){
            answer += el.second;
        }
        return answer;
    }

    void print_cycle(){
        cout << "path:" << endl;
        for(pair<pp,int> el:cycle){
            cout << el.first.first << " - " << el.first.second << endl;
        }
        cout << endl << "MST cost: " << cost() << endl;
    }
private:
    unsigned int total_vertices;
    vector<pp>vertices;
    map<pp,int>edges;
    vector<int>preorder;
    vector<pair<pp,int>>cycle;
    vector<vector<pair<int,int>>>mst;
    vector<bool>visited;

    void create_cycle(int start){
        visited.resize(total_vertices+1);
        for(int i = 1; i <= (int)total_vertices; i++)
            visited[i] = false;

        visited[start] = true;
        create_cycle_path(1);

        for(int i = 0; i < (int)preorder.size() - 1; i++)
            cycle.push_back(make_pair(make_pair(preorder[i], preorder[i+1]), edges[make_pair(min(preorder[i], preorder[i+1]),max(preorder[i], preorder[i+1]))]));
        int lastVertex = preorder[(int)preorder.size()-2];
        cycle.push_back(make_pair(make_pair(preorder[lastVertex], 1), dij(vertices[1],vertices[lastVertex])));
    }

    void create_cycle_path(int vertex){
        preorder.push_back(vertex);
        sort(mst[vertex].begin(), mst[vertex].end());
        for(pair<int,int> neighbor:mst[vertex])
            if(!visited[neighbor.second]){
                visited[neighbor.second] = true;
                create_cycle_path(neighbor.second);
            }
    }

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
            while(!pq.empty() and weight == pq.top().weight){
                pq_element front = pq.top();
                pq.pop();

                if(!inMST[front.vertex]){
                    elements.push_back(front);
                }
            }

            if((int)elements.size() > 0 ){
                int minMSTElement = -1;
                for(int i = 1; i <= total_vertices and minMSTElement == -1; i++)
                    if(inMST[i]){
                        minMSTElement = i;
                    }
                sort(elements.begin(), elements.end(), compare(minMSTElement));

                pq_element front = elements[0];
                for(int i = 1; i < (int)elements.size(); i++)
                    pq.push(elements[i]);

                if(front.vertex != start){
                    inMST[front.vertex] = true;
                    parent[front.vertex] = front.parent;
                }

                for(int i = 1; i <= total_vertices; i++)
                    if(i != front.vertex){
                        int distance = dij(vertices[i], vertices[front.vertex]);
                        if(!inMST[i] and key[i] >= distance){
                            key[i] = distance;
                            pq_element current_el(key[i], i, front.vertex);
                            pq.push(current_el);
                        }
                    }
            }
        }

        mst.resize(total_vertices+1);
        for(int i = 1; i <= total_vertices; i++)
            if(parent[i] != -1){
                int distance = dij(vertices[i], vertices[parent[i]]);
                edges[make_pair(min(i,parent[i]),max(i,parent[i]))] = distance;
                mst[i].push_back(make_pair(distance, parent[i]));
                mst[parent[i]].push_back(make_pair(distance, i));
            }
    }

    static int dij(pp i, pp j){
        int xd = i.first - j.first, yd = i.second - j.second;
        double x = sqrt(xd*xd + yd*yd);
        return (int)(x + 0.5);
    }
};


int main(){
    int n=1;
//    cin >> n;

    ofstream writingfile;
    writingfile.open("saida.txt");

    /* vertices variables */
    unsigned int total_vertices;
    int vertice, x, y;
    vector<pp> vertices;
    /* vertices variables */

    for(int i = 1; i <= n; i++){
        string filename;
        filename = "ent01.txt";//"ent"  + (i<10)?"0":"" + to_string(i) + ".txt";

        ifstream readingfile(filename);

        readingfile >> total_vertices;
        vertices.clear();
        vertices.resize(total_vertices + 1);

        for(unsigned int j = 1; j <= total_vertices; j++){
            readingfile >> vertice >> x >> y;
            vertices[vertice] = make_pair(x,y);
        }
        readingfile.close();

        Solution * solve = new Solution(vertices,total_vertices);
        solve->print_cycle();
        delete solve;
    }
    writingfile.close();

    return 0;
}
