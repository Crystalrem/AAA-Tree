#ifndef DATA_STRUCTURE_AAA_TREE
#define DATA_STRUCTURE_AAA_TREE

#include <bits/stdc++.h>

namespace DataStructure
{
	class Data
	{
	public:
	
		int val;
		Data(){val = 0;}
		Data(int val) : val(val) {}
		~Data() {}
		
		Data operator *(const int &x) const
		{
			Data re;
			re.val = val * x;
			return re;
		}
		
		Data operator +(const Data &x) const
		{
			Data re;
			re.val = val + x.val;
			return re;
		}
		
		void operator *=(const int &x)
		{
			val = val * x;
		}
		
		void operator += (const Data &x)
		{
			val = val + x.val;
		}
		
		Data & operator = (const Data &x)
		{
			if(this == &x) return *this;
			this -> val = x.val;
			return *this;
		}
	
		bool operator != (const Data &x) const
		{
			return val != x.val; 
		}
		
	}zero(0);
	
    class Edge
    {
    public:
	
        int u, v;
        Edge(){}
        Edge(int u, int v) : u(u), v(v) {}
		~Edge() {}
		
    };

    class AAATree
    {
    public:
        class SplayNode;
        
        std::vector<SplayNode*> reflection;
        
        class TreapNode
        {
        public:
		
            TreapNode *left, *right;
            int num, priority;
            Data sum;
            SplayNode *node;
            
            TreapNode() {}
			
            TreapNode(SplayNode *node, int num, int priority) : node(node), num(num), priority(priority)
            {
                sum = zero;
                left = right = NULL;
            }
			
            TreapNode(TreapNode &other)
            {
                num = other.num;
                sum = other.sum;
                priority = other.priority;
                node = NULL;
                left = right = NULL;
            }
            
            ~TreapNode() {}
        };
        
        class SplayNode
        {
        public:
            
            Data data, linksum, subsum, plus, same;
            int size, num;
            bool  reverse;
            SplayNode *child[2], *par;
            TreapNode *node, *root;
            
            SplayNode() {}
			
            SplayNode(Data data, int num) : data(data), num(num)
            {
                size = 1;
                reverse = false;
                child[0] = child[1] = par = NULL;
                plus = same = subsum = zero;
                linksum = data;
                node = new TreapNode(this, num, 0);
                root = NULL;
            }
			
            SplayNode(SplayNode &other)
            {
                num = other.num;
                plus = other.plus;
                same = other.same;
                linksum = other.linksum;
                subsum = other.subsum;
                data = other.data;
                size = other.size;
                reverse = other.reverse;
                child[0] = child[1] = par = NULL;
                node = new TreapNode(*(other.node));
                node -> node = this;
            }
			
            bool isroot()
            {
                return (!par || (par -> child[0] != this && par -> child[1] != this));
            }
			
            ~SplayNode()
            {
                if(node) delete(node);
            }
			
        };
        
public:
        AAATree() {}
		
        AAATree(std::vector <int> datas, std::vector <Edge> edges)
        {
            reflection.push_back(NULL);
            for (int i = 0; i < datas.size(); i++)
                reflection.push_back(new SplayNode(datas[i], i + 1));
            
            for(int i = 0; i < edges.size(); i++)
            	link(edges[i].v, edges[i].u);
        }
		
        AAATree(AAATree &other)
        {
            reflection.push_back(NULL);
            for (int i = 1; i < other.reflection.size(); i++)
                reflection.push_back(new SplayNode(*(other.reflection[i])));
            for (int i = 1; i < other.reflection.size(); i++)
            {
                int tmp;
                if(other.reflection[i] -> root)
                {
                    tmp = other.reflection[i] -> child[0] -> num;
                    reflection[i] -> root = reflection[tmp] -> node;
                }
                if(other.reflection[i] -> child[0])
                {
                    tmp = other.reflection[i] -> child[0] -> num;
                    reflection[i] -> child[0] = reflection[tmp];
                }
                if(other.reflection[i] -> child[1])
                {
                    tmp = other.reflection[i] -> child[1] -> num;
                    reflection[i] -> child[1] = reflection[tmp];
                }
                if(other.reflection[i] -> par)
                {
                    tmp = other.reflection[i] -> par -> num;
                    reflection[i] -> par = reflection[tmp];
                }
                if(other.reflection[i] -> node -> left)
                {
                    tmp = other.reflection[i] -> node -> left -> num;
                    reflection[i]-> node -> left = reflection[tmp] -> node;
                }
                if(other.reflection[i] -> node -> right)
                {
                    tmp = other.reflection[i] -> node -> right -> num;
                    reflection[i]-> node -> right = reflection[tmp] -> node;
                }
            }
        }
        
        ~AAATree()
        {
            for (int i = 1; i < reflection.size(); i++)
                delete reflection[i];
            reflection.clear();
        }
		
private:
       
        void TreapUpdate(TreapNode *t)
        { 
            t -> sum = t -> node -> subsum + t -> node -> linksum;
            if(t -> left != NULL) t -> sum += t -> left -> sum;
            if(t -> right != NULL) t -> sum += t -> right -> sum;
        }
        
        void TreapLL(TreapNode *&node)
        {
            TreapNode *tmp = node -> left;
            node -> left = tmp -> right;
            tmp -> right = node;
            TreapUpdate(node);
            TreapUpdate(tmp);
            node = tmp;
        }
        
        void TreapRR(TreapNode *&node)
        {
            TreapNode *tmp = node -> right;
            node -> right = tmp -> left;
            tmp -> left = node;
            TreapUpdate(node);
            TreapUpdate(tmp);
            node = tmp;
        }
        
        void TreapInsert(TreapNode *&t, SplayNode *node, int num0)
        {
            if(t == NULL)
            {                                 
                int priority = std::rand();
                node -> node -> left = NULL;
                node -> node -> right = NULL;
                t = node -> node;
                t -> sum = node -> linksum + node -> subsum;
                t -> priority = priority;
            }
            else if(num0 < t -> num)
            {   
                TreapInsert(t -> left, node, num0);
                if(t -> left -> priority < t -> priority) TreapLL(t);
            }   
            else if(num0 > t -> num)
            {
                TreapInsert(t -> right, node, num0);
                if(t -> right -> priority < t -> priority) TreapRR(t);
            }
            if(t) TreapUpdate(t);
        }
        
        void TreapRemove(TreapNode *&t, int num0)
        {
            if(t != NULL)
            {
                if(num0 < t -> num) TreapRemove(t -> left, num0);
                else if(num0 > t -> num) TreapRemove(t -> right, num0);
                else
                {
                    if(t -> left == NULL)
                    {
                        TreapNode* tmp = t;
                        t = t -> right; 
                    }
                    else if(t -> right == NULL)
                    {
                        TreapNode* tmp = t;
                        t = t -> left;
                    }
                    else
                    {
                        if(t -> left -> priority < t -> right -> priority) TreapLL(t);
                        else TreapRR(t);
                        TreapRemove(t, num0);
                    }
                }
                if(t) TreapUpdate(t);
            }
        }
        

    private: 
	
        void Plus(SplayNode *x, Data val)
        {
            Data tmp = x -> plus + val;
            x -> plus = zero;
            if(x -> same !=  zero) 
            {
                for (int i = 0; i <= 1; i++)
                if(x -> child[i]) Same(x -> child[i], x -> same);
                x -> same = zero;
            }
            x -> plus = tmp;
            x -> linksum += val * x -> size;
            x -> data += val; 
        }
        void Same(SplayNode *x, Data val)
        {
            x -> plus = zero;
            x -> same = val;
            x -> linksum = val * x -> size;
            x -> data = val;
        }
        void Reverse(SplayNode *x)
        {
            x -> reverse ^= 1;
            std::swap(x -> child[0], x -> child[1]);
        }
        void SplayUp(SplayNode *x)
        {
            SplayDown(x);
            x -> size = 1;
            if (x -> child[0]) x -> size += x -> child[0] -> size;
            if (x -> child[1]) x -> size += x -> child[1] -> size;
            
            x -> linksum = x -> data;
            if (x -> child[0]) x -> linksum += x -> child[0] -> linksum;
            if (x -> child[1]) x -> linksum += x -> child[1] -> linksum;
            
            x -> subsum = 0;
            if (x -> root) x -> subsum += x -> root -> sum;
            for (int i = 0; i <= 1; i++)
                if(x -> child[i]) 
                    x -> subsum += x -> child[i] -> subsum;
        }
        
        void SplayDown(SplayNode *x)
        {
            if(!x) return;
            if (x -> reverse)
            {
                for (int i = 0; i <= 1; i++)
                if (x -> child[i]) Reverse(x -> child[i]);
                x -> reverse = false;
            }
            if (x -> same != zero)
            { 
                for (int i = 0; i <= 1; i++)
                if (x -> child[i])
                {
                    x -> child[i] -> plus = zero;
                    Same(x -> child[i], x -> same);
                }
                x -> same = zero;
            }
            if (x -> plus != zero)
            {
                for (int i = 0; i <= 1; i++)
                if (x -> child[i]) Plus(x -> child[i], x -> plus);
                x -> plus = zero;
            }
        }
        
        
        void SplayLrot(SplayNode *x)
        {
            SplayNode *p = x -> par;
            SplayDown(p);
            SplayDown(x);
            x -> par = p -> par;
            if (p -> par != NULL)
            {
                if (p -> par -> child[0] == p) p -> par -> child[0] = x;
                if (p -> par -> child[1] == p) p -> par -> child[1] = x;
            }
            p -> par = x;
            if(x -> child[1]) x -> child[1] -> par = p;
            p -> child[0] = x -> child[1];
            x -> child[1] = p;
            SplayUp(p);
            SplayUp(x);
        }
		
        void SplayRrot(SplayNode *x)
        {
            SplayNode *p = x -> par;
            SplayDown(p); 
            SplayDown(x);
            x -> par = p -> par;
            if (p -> par != NULL)
            {
                if (p -> par -> child[0] == p) p -> par -> child[0] = x;
                if (p -> par -> child[1] == p) p -> par -> child[1] = x;
            }
            p -> par = x;
            if(x -> child[0]) x -> child[0] -> par = p;
            p -> child[1] = x -> child[0];
            x -> child[0] = p;
            SplayUp(p);
            SplayUp(x);
        }
        
        
    public:
        void AAA_remove(SplayNode *tar, int id)
        {
            maintain(tar);
            TreapRemove(tar -> root, id);
            maintain(tar);
            while (!tar -> isroot())
            {
                tar = tar -> par;
                maintain(tar);
            }
        }
        
        void AAA_insert(SplayNode *tar, TreapNode *&root, SplayNode *ctx, int num)
        {
            maintain(tar);
            maintain(ctx);
            ctx -> node -> left = NULL;
            ctx -> node -> right = NULL;
            TreapInsert(root, ctx, num);
            maintain(tar);
            while (!tar -> isroot())
            {
                tar = tar -> par;
                maintain(tar);
            }
        }

        void maintain(SplayNode *x)
        {
            SplayDown(x);
            SplayUp(x);
        }

        void pushdown(SplayNode *x)
        {
            SplayDown(x);
        }
		
        void splay(SplayNode *x)
        {
            SplayDown(x);
            if (x -> isroot()) return;
            while (!x -> isroot())
            {
                if(!x -> par -> isroot()) SplayDown(x -> par -> par);
                SplayDown(x -> par); 
                SplayDown(x);
                if (x -> par -> isroot())
                {
                    if (x -> par -> child[0] == x)
                        SplayLrot(x);
                    else
                        SplayRrot(x);
                }
                else
                {
                    SplayNode *dad = x -> par, *grandpa = x -> par -> par;
                    bool iAmLeft = (dad -> child[0] == x), dadIsLeft = (grandpa -> child[0] == dad);
                    if (iAmLeft == dadIsLeft)
                    {
                        if (iAmLeft)
                        {
                            SplayLrot(dad);
                            SplayLrot(x);
                        }
                        else
                        {
                            SplayRrot(dad);
                            SplayRrot(x);
                        }
                    }
                    else
                    {
                        if (iAmLeft)
                        {
                            SplayLrot(x);
                            SplayRrot(x);
                        }
                        else
                        {
                            SplayRrot(x);
                            SplayLrot(x);
                        }
                    }
                }
            }
        }
        
      	SplayNode* my_access(SplayNode *x)
        {
            splay(x);
            if(x -> child[0])
            {
                SplayNode *tmp = x -> child[0];
                splay(x);
                x -> child[0] = NULL;
                maintain(tmp);
                while(tmp -> child[1]) {
					tmp = tmp -> child[1];
					maintain(tmp);
				}
                AAA_insert(x, x -> root, tmp, tmp -> num);
            }
            
            SplayNode *y = x;
            if(x -> par) 
            {
            	SplayNode *tmp = x -> par;	
                y = my_access(x -> par);
                
                splay(x);
			    SplayNode *tmp1 = x;
			    maintain(x);
			    while(tmp1 -> child[1]) {
			    	tmp1 = tmp1 -> child[1];
			    	maintain(tmp1);
			    }
			    
			    x -> par -> child[0] = x;
                AAA_remove(x -> par, tmp1 -> num);    
            }
            splay(x);
            return y;
        }

    public:
	
        SplayNode* get_true_father(SplayNode *x) 
        {
        	splay(x);
            assert(x -> isroot());
            maintain(x);
            if(x -> child[1]) 
            {
                return x -> par;
            } 
            else 
            {
                x = x -> child[1];
                maintain(x);
                while(x -> child[0]) 
                {
                    x = x -> child[0];
                    maintain(x);
                }
                return x;
            }
        }
		
        SplayNode* my_evert(SplayNode *x) 
        {
            SplayNode *tmp = my_access(x);
            Reverse(x);
            return tmp;
        }
        
        void access(int x)
        {
            my_access(reflection[x]);
            return ;
        } 
		
        int lca(int x, int y) {
             my_evert(reflection[x]);
             SplayNode *ans = my_access(reflection[y]);
             return ans -> num;
        }
		
        void cut(int now)
        {
            SplayNode* x = reflection[now];
            SplayNode* y = get_true_father(x);
            my_evert(y);
            my_access(x);
            splay(y);
            for(int i = 0; i <= 1; i++) 
            {
                if(y -> child[i] == x) 
                {
                    x -> par = NULL;
                    y -> child[i] = NULL;
                }
            }
            maintain(y);
        	maintain(x);
        }
        
        void evert(int x)
        {
            my_evert(reflection[x]);
            return ;
        }
		
        void link(int nowx, int nowy)
        {
            SplayNode *x = reflection[nowx];
            SplayNode *y = reflection[nowy];
            my_access(x);
 			SplayNode *z = my_access(y);
            splay(x);
            if(x -> par == z || x == z) return ;
 			my_evert(x);
            x -> par = y;
           	maintain(y);
		    AAA_insert(y, y -> root, x, x -> num);
            maintain(y);
			return ;
        }
       
        Data query(int nowx)
        {
            SplayNode *x = reflection[nowx];
            SplayNode *y = get_true_father(x);
            if(y) my_access(y);
            splay(x);
            return x -> subsum + x -> linksum;
        }
        
       	Data query(int nowx, int nowy)
        {
            evert(nowx);
            access(nowy);           
            SplayNode *y = reflection[nowy];
            splay(y);
            Data ans = y -> linksum;
            return ans;
        }
       
        void modify(int nowx, int nowy, int value)
        {
            evert(nowx);
            access(nowy);
            SplayNode *x = reflection[nowx];
            SplayNode *y = reflection[nowy];
            splay(y);
            Same(y, value);
            return; 
        }
  
        void plus(int nowx, int nowy, int val)
        { 
            evert(nowx);
            access(nowy);
            SplayNode *x = reflection[nowx];
            SplayNode *y = reflection[nowy];
            splay(y);
            Plus(y, val);
            return; 
        }
    };
}

#endif
