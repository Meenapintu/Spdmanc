#include <simplecpp>
#include <string>
#include <map>
#include <sstream>

struct Node{
    double weight;
    int node_id;
    int trabled;
    //vector<Node*> childs;
    Node *child;
    Node *parent;
};

const int npoints = 12;
vector<Circle*> points;
vector<vector<double> > wt(npoints, vector<double>(npoints));

/**                                     **/
int dest =11;
int source=0;
int min_w=244443434;
Node* min_p=NULL;

bool opt_travel(Node **node,string color);

struct Node* get_node(int node_id)
{
	struct Node *temp = new Node;
	temp->node_id = node_id;
	temp->parent=NULL;
	temp->weight=0;
	temp->trabled=1;
	return temp;
}



bool is_travled(Node **node,int node_id)
{
    bool travled = false;
    while(*node!=NULL)
    {
        if((*node)->node_id == node_id)
        {
            travled=true;break;
        }
        node = &((*node)->parent);
    }
    return travled;
}
bool deleter(Node* node){
    while(node->child!=NULL &&node->child->child!=NULL)
    {
        Node *temp = node->child->child;
        delete node->child;
        node->child = temp;
    }
    if(node->child!=NULL &&node->child->child==NULL)
    {
        delete node->child;
        node->child=NULL;
    }
    return true;
}

bool trable(Node* parent)
{
    /** we know that the source is going to find only outgong path and avid looping **/
        int pid = parent->node_id;
        bool yes=false;
        for(int i=0;i<points.size();i++ )
        {
            if(!is_travled(&parent,i))
            {
                struct Node *temp = new Node;
                temp->node_id = i;
                temp->trabled=parent->trabled+1;
                temp->weight = parent->weight+wt[pid][i];
                temp->parent = parent;
                parent->child = temp;
                if(parent->child->node_id !=dest)
                {
                   if(!trable(parent->child))
                    deleter(parent);
                   else yes=true;
                }
                else
                {
                    if(parent->child->trabled==npoints)
                    {
                        if(parent->child->weight < min_w)
                        {
                            min_w=parent->child->weight;
                            min_p = parent->child;
                            opt_travel(&min_p,"red");
                            yes=true;
                        }
                        else
                        {
                        delete parent->child;
                        parent->child=NULL;
                        }
                    }
                    else
                    {
                        delete parent->child;
                        parent->child=NULL;
                    }
                }
            }
            //else{cout<<"trabled\n";}

        }
        return yes;
}

bool opt_travel(Node **node,string color)
{
    bool travled = false;
    while((*node)->parent!=NULL &&(*node)!=NULL)
    {
        Line l = Line(points[(*node)->node_id]->getOrigin().getX(), points[(*node)->node_id]->getOrigin().getY(),
            points[(*node)->parent->node_id]->getOrigin().getX(), points[(*node)->parent->node_id]->getOrigin().getY());
        l.setColor(COLOR(color.c_str()));
        l.setZIndex(10);
        l.imprint(false);
        node = &((*node)->parent);
    }
    return travled;
}

int main(){
  initCanvas();

  repeat(npoints){        // click points on the screen
    int pos = getClick();
    points.push_back(new Circle(pos/65536, pos%65536, 5));
  }

  // Generate the distance matrix, wt.
  for(size_t i=0; i<points.size(); i++){
    for(size_t j=0; j<points.size(); j++){
      wt[i][j] = sqrt(pow(points[i]->getOrigin().getX()-
			  points[j]->getOrigin().getX(),2) +
		      pow(points[i]->getOrigin().getY()-
			  points[j]->getOrigin().getY(),2));
    }
  }


  // your tsp code here
  //

        Node *base = get_node(source);
        trable(base);
        opt_travel(&min_p,"green");


  // sample code to show the tsp.  This code just connects points in
  // the order they were given.
 /* for(int i=0; i<npoints-1; i++)
    Line(points[i]->getOrigin().getX(), points[i]->getOrigin().getY(),
	 points[i+1]->getOrigin().getX(), points[i+1]->getOrigin().getY()).imprint();*/
	  cout<<"DONE\n";
  getClick();


}
