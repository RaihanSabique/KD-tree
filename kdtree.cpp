#include<bits/stdc++.h>
#include<math.h>
#include<queue>
#include <sstream>
using namespace std;

struct point2D{
    double x;
    double y;
};

class rect{
public:
    double xmin;
    double xmax;
    double ymin;
    double ymax;

    public:
    rect()
    {

    }

    void setRect(double xmin, double ymin, double xmax, double ymax) {
        if (xmax < xmin || ymax < ymin) {
            cout<<"Invalid rectangle"<<endl;;
        }
        this->xmin = xmin;
        this->ymin = ymin;
        this->xmax = xmax;
        this->ymax = ymax;
    }

    bool checkIntersection(rect r) {
        return this->xmax >= r.xmin && this->ymax >= r.ymin
            && r.xmax >= this->xmin && r.ymax >= this->ymin;
    }

    double distanceSquaredTo(point2D p) {
        double dx = 0.0, dy = 0.0;
        if      (p.x < xmin) dx = p.x - xmin;
        else if (p.x > xmax) dx = p.x - xmax;
        if      (p.y < ymin) dy = p.y - ymin;
        else if (p.y > ymax) dy = p.y - ymax;
        return dx*dx + dy*dy;
    }

    bool contains(point2D p) {
        return (p.x >= xmin) && (p.x <= xmax) && (p.y >= ymin) && (p.y <= ymax);
    }


};

struct KDnode
{
        point2D p;
        rect r;

        KDnode *leftNode;
        KDnode *rightNode;

};

class KDtree{
    private:
    int size;
    KDnode *root;

    public:
    KDtree()
    {
        this->size = 0;
        this->root=NULL;
    }

    bool isEmpty() {
        return this->size == 0;
    }
    int treeSize() {
        return this->size;
    }

    void insertPoint(point2D p)
    {

        root=insert(root,p,-100.0,-100.0,100.0,100.0,true);
    }

    queue<point2D> rangeQuery(rect rct) {
        queue<point2D> q;
        int cnt=0;
        range(root, rct, q,cnt);
        //cout<<q.front().x<<" "<<q.front().y<<endl;
        //cout<<cnt<<endl;
        //cout<<"Inside rangeQuery:"<<q.size()<<endl;
        return q;
    }

    point2D nearest(point2D p) {

        //if (root == NULL) return NULL;
        return nearestNeighbor(root, p, root->p, true);
    }

    void printTree()
    {
        cout<<root->rightNode->p.x<<" "<<root->rightNode->p.y<<endl;
        cout<<root->rightNode->r.xmin<<" "<<root->rightNode->r.ymin<<"-->"<<root->rightNode->r.xmax<<" "<<root->rightNode->r.ymax<<endl;
    }

    private:
        KDnode* insert(KDnode *node,point2D p,double x0,double y0,double x1,double y1,bool xcmp)
        {
            if (node == NULL) {
                this->size++;
                KDnode *Node=new KDnode;
                Node->p=p;
                Node->r.setRect(x0,y0,x1,y1);

                Node->leftNode=NULL;
                Node->rightNode=NULL;
                //cout<<"here"<<endl;

                return Node;
            }
            else if (node->p.x == p.x && node->p.y == p.y) return node;

            if (xcmp) {
                double cmp = p.x - node->p.x;
                if (cmp < 0)
                    node->leftNode = insert(node->leftNode, p, x0, y0, node->p.x, y1, !xcmp);
                else
                    node->rightNode = insert(node->rightNode, p, node->p.x, y0, x1, y1, !xcmp);
            }
            else
            {
                double cmp = p.y - node->p.y;
                if (cmp < 0)
                    node->leftNode = insert(node->leftNode, p, x0, y0, x1, node->p.y, !xcmp);
                else
                    node->rightNode = insert(node->rightNode, p, x0, node->p.y, x1, y1, !xcmp);
            }


            return node;
        }

        void range(KDnode *node, rect r, queue<point2D> q,int cnt) {
            if (node == NULL) return;
            if (r.contains(node->p)) {
                q.push(node->p);
                cnt++;
                cout<<"("<<node->p.x<<" "<<node->p.y<<")"<<endl;

            }
            if (r.checkIntersection(node->r)) {
                range(node->leftNode, r, q, cnt);
                range(node->rightNode, r, q, cnt);
            }

    }
    public:
    double distanceSquaredTo(point2D p1,point2D p2)
    {
        double dx = 0.0, dy = 0.0;
        if      (p1.x > p2.x) dx = p1.x - p2.x;
        else if (p1.x < p2.x) dx = p2.x - p1.x;
        if      (p1.y > p2.y) dy = p1.y - p2.y;
        else if (p1.y < p2.y) dy = p2.y - p1.y;
        return dx*dx + dy*dy;
    }


    point2D nearestNeighbor(KDnode *node, point2D p, point2D c, bool xcmp) {
        point2D closest = c;
         if (node == NULL) return closest;
         if (distanceSquaredTo(node->p,p) < distanceSquaredTo(closest,p))
            closest = node->p;

        if (node->r.distanceSquaredTo(p) < distanceSquaredTo(closest,p)) {
            KDnode *near;
            KDnode *far;
            if ((xcmp && (p.x < node->p.x)) || (!xcmp && (p.y < node->p.y))) {
                near = node->leftNode;
                far = node->rightNode;
            }
            else {
                near = node->rightNode;
                far = node->leftNode;
            }

            closest = nearestNeighbor(near, p, closest, !xcmp);

            closest = nearestNeighbor(far, p, closest, !xcmp);
        }
        return closest;
    }



};

int main()
{
    ifstream in ("input.txt");

    KDtree tree;

    double a,b;
    int x,n;

    if(in>>x)
    {
        n=x;

    }
    point2D points[n];
    int i=0;
    while(in>>a>>b)
    {
        points[i].x=a;
        points[i].y=b;
        i++;
    }
    for(int j=0;j<n;j++)
    {
        //cout<<points[j].x<<" "<<points[j].y<<endl;
        tree.insertPoint(points[j]);
    }

	//struct Node *root = NULL;


	//int n = sizeof(points)/sizeof(points[0]);

	for (int i=0; i<n; i++){

	}
	//cout<<"Tree:"<<endl;
	//tree.printTree();
	string line;
	    //cout<<root->point[0]<<" "<<root->point[1]<<endl;
//    print_tree(root);
    char c;
    double x1,y1,x2,y2;
    ifstream in1 ("input.txt");
    //cout<<line<<endl;
    int lineno=0;
    while(getline(in1,line))
    {
        lineno++;
        if(lineno==n+2)
        {
            n++;
            if(line[0]=='R')
            {
                cout<<line<<endl;
                stringstream iss;
                iss<<line;
                double arr[4];
                int i=0;
                string temp;
                double found;
                while (!iss.eof()) {
                    iss >> temp;
                    if (stringstream(temp) >> found){
                        //cout << found <<endl;
                        arr[i]=found;
                        i++;
                    }

                    temp = "";
                }


                //cout<<arr[3]<<endl;

                queue<point2D> qu;
                rect r;
                r.setRect(arr[0],arr[1],arr[2],arr[3]);
                //cout<<r.ymax<<endl;
                qu=tree.rangeQuery(r);
                //cout<<"Points in range:"<<qu.size()<<endl;
                while(!qu.empty())
                {
                    cout<<qu.front().x<<" "<<qu.front().y<<endl;
                    qu.pop();
                }
            }
            if(line[0]=='N')
            {
                cout<<line<<endl;
                stringstream iss;
                iss<<line;
                double arr[2];
                int i=0;
                string temp;
                double found;
                while (!iss.eof()) {
                    iss >> temp;
                    if (stringstream(temp) >> found){
                        //cout << found <<endl;
                        arr[i]=found;
                        i++;
                    }

                    temp = "";
                }

                point2D p,nearest;
                p.x=arr[0];
                p.y=arr[1];
                //cout<<p.x<<" "<<p.y<<endl;

                nearest=tree.nearest(p);
                cout<<sqrt(tree.distanceSquaredTo(nearest,p))<<"("<<nearest.x<<" "<<nearest.y<<")"<<endl;
            }

        }



    }

    return 0;

}
