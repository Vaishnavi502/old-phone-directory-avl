#include <bits/stdc++.h>
#include <algorithm>
#include <sys/time.h>
using namespace std;

long gettime()
{
    struct timeval start;
    gettimeofday(&start,NULL);
    return start.tv_sec*1000000+start.tv_usec;
}
// DLL node
class llnode;
// Tree Node
class node
{
    protected:
        string fn, ln; // First name, last name
        long int ph; // 10-digit phone no.
    public:
        node *left, *right;
        void check(long int phno)
        {
            while(ph/10e10 > 0.09 || ph/10e10 < 0.01)
            {
                cout<<"Valid phone number must contain 10 digits.\nRe-enter phone number: ";
                cin>>ph;
            }
        }
        void check(string* n)
        {
            while(regex_match(*n, regex("^[A-Za-z]+$")) == false)
            {
                cout<<"Name should contain only alphabets.\nRe-enter name: ";
                cin>>*n;
            }
        }
        node()
        {
            fn = ln = "";
            ph = 0;
            left = NULL;
            right = NULL;
        }
        node(int p)
        {
            cout<<"Enter name in format:'[FIRSTNAME] [LASTNAME]': ";
            cin>>fn>>ln;
            check(&fn);
            check(&ln);
            cout<<"Enter phone number: ";
            cin>>ph;
            check(ph);
            left = NULL;
            right = NULL;
        }
        node(node* n)
        {
            fn = n->fn;
            ln = n->ln;
            ph = n->ph;
            left = NULL;
            right = NULL;
        }
        void dispdet()
        {
            cout<<"\nNAME: "<<fn<<" "<<ln<<endl;
            cout<<"Phone No.: "<<ph<<endl;
        }
        void setfn(string fn)
        {
            this->fn = fn;
        }
        void setln(string ln)
        {
            this->ln = ln;
        }
        void setph(long int ph)
        {
            this->ph = ph;
        }
        friend node* newcontact(node*);
        friend node* splay(node*,long int);
        friend node *insert(node *curroot, node* root);
        friend node* delcontact(node*, long int);
        friend int addtofile();
        friend void delfromfile(node*,string);
        friend void delfromfile(node*,long int);
        friend int updfile(node*,long int,long int);
        friend int updfile(node*,string,string,int);
        friend void recentlist(node* root);
        friend class llnode;
        friend class avlTree;
}*root;

class avlTree:public node
{
    public:
        int height(node *);
        int diff(node *);
        node *rr_rotation(node *);
        node *ll_rotation(node *);
        node *lr_rotation(node *);
        node *rl_rotation(node *);
        node* balance(node *);
        node* insertdll(node *, node* );
        void display(node *, int);
        node* search(node* r,long int phno)
        {
            if(r->ph == phno || r == NULL)
                return r;
            else if(r->ph < phno)
            {
                return search(r->right,phno);
            }
            else
            {
                return search(r->left,phno);
            }
        }
        avlTree()
        {
            root=NULL;
        }
};
/*
 * Height of AVL Tree
 */
int avlTree::height(node *temp)
{
    int h = 0;
    if (temp != NULL)
    {
        int l_height = height (temp->left);
        int r_height = height (temp->right);
        int max_height = max (l_height, r_height);
        h = max_height + 1;
    }
    return h;
}

/*
 * Height Difference
 */
int avlTree::diff(node *temp)
{
    int l_height = height (temp->left);
    int r_height = height (temp->right);
    int b_factor= l_height - r_height;
    return b_factor;
}

/*
 * Right- Right Rotation
 */
node *avlTree::rr_rotation(node *parent)
{
    node *temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}
/*
 * Left- Left Rotation
 */
node *avlTree::ll_rotation(node *parent)
{
    node *temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}

/*
 * Left - Right Rotation
 */
node *avlTree::lr_rotation(node *parent)
{
    node *temp;
    temp = parent->left;
    parent->left = rr_rotation (temp);
    return ll_rotation (parent);
}

/*
 * Right- Left Rotation
 */
node *avlTree::rl_rotation(node *parent)
{
    node *temp;
    temp = parent->right;
    parent->right = ll_rotation (temp);
    return rr_rotation (parent);
}

/*
 * Balancing AVL Tree
 */
node *avlTree::balance(node *temp)
{
    int bal_factor = diff (temp);
    if (bal_factor > 1)
    {
        if (diff (temp->left) > 0)
            temp = ll_rotation (temp);
        else
            temp = lr_rotation (temp);
    }
    else if (bal_factor < -1)
    {
        if (diff (temp->right) > 0)
            temp = rl_rotation (temp);
        else
            temp = rr_rotation (temp);
    }
    return temp;
}

/*
 * Insert Element into the tree
 */
node *avlTree::insertdll(node *root, node* cur)
{
    if (root == NULL)
    {
        root = new node(cur);
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    else if (cur->ph < root->ph)
    {
        root->left = insert(root->left, cur);
        root = balance (root);
    }
    else if (cur->ph >= root->ph)
    {
        root->right = insert(root->right, cur);
        root = balance (root);
    }
    return root;
}

/*
 * Display AVL Tree
 */
void avlTree::display(node *ptr, int level)
{
    int i;
    if (ptr!=NULL)
    {
        display(ptr->right, level + 1);
        cout<<"\n";
        if (ptr == root)
        cout<<"Root -> ";
        for (i = 0; i < level && ptr != root; i++)
            cout<<"        ";
        cout<<ptr->fn<<" "<<ptr->ln<<" "<<ptr->ph;
        display(ptr->left, level + 1);
    }
}

// DLL node
class llnode:public node
{
    public:
        llnode(node* c)
        {
            fn=c->fn;
            ln=c->ln;
            ph=c->ph;
        }
        llnode* next;
        llnode* prev;
        friend llnode* merge(llnode*, llnode*);
        friend void insert(llnode**, long int);
        friend void print(llnode*,int);

};

llnode *split(llnode *head);

// Function to merge two linked lists
llnode *merge(llnode *first, llnode *second)
{
    // If first linked list is empty
    if (!first)
        return second;

    // If second linked list is empty
    if (!second)
        return first;

    // Pick the smaller value
    if (first->fn < second->fn)
    {
        first->next = merge(first->next,second);
        first->next->prev = first;
        first->prev = NULL;
        return first;
    }
    else
    {
        second->next = merge(first,second->next);
        second->next->prev = second;
        second->prev = NULL;
        return second;
    }
}

llnode *mergeSort(llnode *head)
{
    if (!head || !head->next)
        return head;
    llnode *second = split(head);

    // Recur for left and right halves
    head = mergeSort(head);
    second = mergeSort(second);

    // Merge the two sorted halves
    return merge(head,second);
}

// insert at beginning
void insertdll(llnode **head, node* copy)
{
    llnode *temp = new llnode(copy);
    temp->next = temp->prev = NULL;
    if (!(*head))
        (*head) = temp;
    else
    {
        temp->next = *head;
        (*head)->prev = temp;
        (*head) = temp;
    }
}

void print(llnode *head,int order)
{
    head = mergeSort(head);
    llnode *temp = head,*head2=head;
    if(order == 0)
    {
        while (head2)
        {
            temp = head2;
            head2 = head2->next;
        }
        while (temp)
        {
            cout<<temp->fn<<" "<<temp->ln<<"\t\t\t"<<temp->ph<<endl;
            temp = temp->prev;
        }
    }
    else // default case
    {
        while (head2)
        {
            cout<<head2->fn<<" "<<head2->ln<<"\t\t\t"<<head2->ph<<endl;
            head2 = head2->next;
        }
    }
}

void swap(long int *A, long int *B)
{
    long int temp = *A;
    *A = *B;
    *B = temp;
}

// Split a doubly linked list (DLL) into 2 DLLs of half sizes
llnode *split(llnode *head)
{
    llnode *fast = head,*slow = head;
    while (fast->next && fast->next->next)
    {
        fast = fast->next->next;
        slow = slow->next;
    }
    llnode *temp = slow->next;
    slow->next = NULL;
    return temp;
}

// Allocate a new node accepting values for all fields, NULL to left and right pointers.
node* newcontact(node* newn)
{
    node* Node = NULL;
    if(newn == NULL)
        Node = new node();
    else
    {
        Node = new node(newn);
        Node->left = Node->right = NULL;
    }
    return (Node);
}

// Appending new contact to file in next line
int addtofile()
{
    ofstream wrtoph("phone.txt",ios::app);
    if(!wrtoph)
    {
        cout<<"file not opened properly"<<endl;
        return -1;
    }
    // newnode create -- write to file
    node* root = new node(1);
    wrtoph<<root->fn<<" ";
    wrtoph<<root->ln<<" ";
    wrtoph<<root->ph<<"\n";
    wrtoph.close();
    return 0;
}


// Deleting a contact using phone number
void delfromfile(node* root, long int phone)
{
    ifstream ph("phone.txt",ios::in);
    ofstream t("temp.txt",ios::out);
    string fir,las; long int p;
    while(!ph.eof())
    {
        ph>>fir;
        ph>>las;
        ph>>p;
        if(p != phone)
        {
            t<<fir<<" "<<las<<" "<<p<<"\n";
        }
    }
    remove("phone.txt");
    rename("temp.txt","phone.txt");
}

// Updating contact's phone number
int updfile(node* r,long int phno,long int oldph)
{
    ifstream f("phone.txt",ios::in);
    ofstream newf("new.txt",ios::app);
    string line,w1,w2; long int no;
    while(getline(f,line))
    {
        f>>w1>>w2;
        f>>no;
        if(oldph != no)
        {
            newf<<w1<<" "<<w2<<" "<<no<<"\n";
        }
        else
        {
            newf<<w1<<" "<<w2<<" "<<phno<<"\n";
        }
    }
    remove("phone.txt");
    rename("new.txt","phone.txt");
    return 0;
}

// Updating contact's name
int updfile(node* r,string w,string oldn)
{
    ifstream f("phone.txt",ios::in);
    ofstream newf("new.txt",ios::out);
    string line;
    while(getline(f,line))
    {
        if(line.substr(0,oldn.size()) != oldn)
            newf<<line<<"\n";
        else
        {
            newf<<w<<line.substr(oldn.size(),(line.size()-oldn.size()))<<"\n";
        }
    }
    remove("phone.txt");
    rename("new.txt","phone.txt");
    return 0;
}

// Right-rotating a y-rooted subtree.
node *rightRotate(node *x)
{
    node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// Right-rotating a x-rooted subtree
node *leftRotate(node *x)
{
    node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// If the key is present in the tree, this function moves it to the root.
// If the key is not present, it returns the last item accessed by root.
// This function modifies the tree and returns the modified root.
node *splay(node *root, long int searph)
{
    // Root is NULL or key is present at root
    if (root == NULL || root->ph == searph)
        return root;

    // Key lies in left subtree
    if (root->ph > searph)
    {
        // Key is not in tree
        if (root->left == NULL)
            return root;

        // Zig-Zig (Left Left)
        if (root->left->ph > searph)
        {
            // splay node to left-left of root
            root->left->left = splay(root->left->left, searph);

            // Do the first rotation for the root, followed by the second rotation.
            root = rightRotate(root);
        }
        else if (root->left->ph < searph) // Zig-Zag (Left Right)
        {
            //  splay node to left-right of root
            root->left->right = splay(root->left->right, searph);

            // Do first rotation for root->left
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        // Do second rotation for root
        return (root->left == NULL) ? root : rightRotate(root);
    }
    // Key lies in right subtree
    else
    {
        if (root->right == NULL)
            return root;

        // Zag-Zig (Right Left)
        if (root->right->ph > searph)
        {
            // splay node to right-left of root
            root->right->left = splay(root->right->left, searph);

            // Do first rotation for root->right
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        // Zag-Zag (Right Right)
        else if (root->right->ph < searph)
        {
            // splay node to right-right of root and do first rotation
            root->right->right = splay(root->right->right, searph);
            root = leftRotate(root);
        }

        // Do second rotation for root and return new root
        return (root->right == NULL) ? root : leftRotate(root);
    }
}

// If a key is present in the tree, it is splayed to root
node *bstSearch(node *root, long int searph)
{
    return splay(root, searph);
}

// Function to insert a new key k in splay tree with given root
node *insert(node *curroot, node* root)
{
    // Simple Case: If tree is empty
    if (curroot == NULL) return newcontact(root);

    // Bring the closest leaf node to root
    curroot = splay(curroot, root->ph);
    // If key is already present, then return
    if (curroot->ph == root->ph) return curroot;

    // Otherwise allocate memory for new node
    node *newnode = newcontact(root);

    // If root's key is greater, make root as right child of newnode and copy the left child of root to newnode
    if (curroot->ph > root->ph)
    {
        newnode->right = curroot;
        newnode->left = curroot->left;
        curroot->left = NULL;
    }

    // If root's key is smaller, make root as left child of newnode and copy the right child of root to newnode
    else
    {
        newnode->left = curroot;
        newnode->right = curroot->right;
        curroot->right = NULL;
    }

    return newnode; // newnode becomes new root
}

node* delcontact(node* root,long int phone)
{
    delfromfile(root,phone);
    node *temp;
	if (!root)
		return NULL;

	// Splay the given key
	root = splay(root, phone);

	// If key is not present, then
	// return root
	if (phone != root->ph)
		return root;

	// If key is present
	// If left child of root does not exist
	// make root->right as root
	if (!root->left)
	{
		temp = root;
		root = root->right;
	}

	// Else if left child exits
	else
	{
		temp = root;

		/*maximum node in left subtree will get splayed*/
		root = splay(root->left, phone);

		// Make right child of previous root as new root's right child
		root->right = temp->right;
	}

	// free the previous root node, that is, the node containing the key
	free(temp);

	// return root of the new Splay Tree
	return root;
}


// Preorder traversal of splay tree
void preOrder(node *root)
{
    if (root != NULL)
    {
        root->dispdet();
        if(root->left != NULL)
            cout<<"\t|\n\tL:\n";
        preOrder(root->left);
        if(root->right != NULL)
            cout<<"\t|\n\tR:\n";
        preOrder(root->right);
    }
}

void recentlist(node* root)
{
    if(root!= NULL)
    {
        cout<<root->fn<<" "<<root->ln<<"\t\t\t"<<root->ph<<endl;
        recentlist(root->left);
        recentlist(root->right);
    }
}
int main()
{
    int loop_times=0;
    long splay_ins=0,dll_ins=0,avl_ins=0;
    float splaysearch=0,avlsearch=0;
    // MENU driven program for features
    do
    {
        avlTree avl;
        llnode* head = NULL;
        node *curroot = NULL; // Current root after each operation
        // Create file
        string w1,w2; // Text file is space separated
        long int ph;
        node newn;
        ifstream phone("phone.txt", ios::in);
        if(phone.eof())
        {
            cout<<"file not opened properly"<<endl;
            return -1;
        }
        // read each node from  file
        while(!phone.eof())
        {
            phone>>w1;
            phone>>w2;
            phone>>ph;
            // Updating new node to insert into file
            newn.setfn(w1);
            newn.setln(w2);
            newn.setph(ph);
            long splins=gettime();
            curroot = insert(curroot,&newn);
            long endsplins=gettime();
            splay_ins+=(endsplins-splins);
            long avlins=gettime();
            root = avl.insertdll(root,&newn);
            long endavl=gettime();
            avl_ins+=(endavl-avlins);
            long dllins=gettime();
            insertdll(&head,curroot);
            long enddllins=gettime();
            dll_ins+=(enddllins-dllins);

        }
        if(loop_times == 0)
        {
            cout<<"\n----------Constructed splay tree:----------\n";
            preOrder(curroot);
        }
        phone.close();
        int opt;
        cout<<"\n\n~~~MENU~~~\n\n1.Create new contact\n2.Delete contact\n3.Update contact\n4.Search by phone no.\n5.Recent Contacts List\n6.Alphabetic Listing of Contacts\n7.Reverse Listing of Contacts\n8.AVL Tree display\n9.Compare DLL, AVL & Splay tree\n10.Quit\nEnter your choice: ";
        cin>>opt;
        switch(opt)
        {
            case 1:
            // 1.Create new contact
            {
                addtofile();
                string w1,w2; // Text file is space separated
                long int ph;
                node newn;
                ifstream phone("phone.txt", ios::in);
                if(phone.eof())
                {
                    cout<<"file not opened properly"<<endl;
                    return -1;
                }
                // read each node from  file
                while(!phone.eof())
                {
                    phone>>w1;
                    phone>>w2;
                    phone>>ph;
                    // Updating new node to insert into file
                    newn.setfn(w1);
                    newn.setln(w2);
                    newn.setph(ph);
                    curroot = insert(curroot,&newn);
                }
                phone.close();
                cout<<"\n----------Splay tree after insertion:----------\n";
                preOrder(curroot);
                cout<<endl;
                break;
            }
            case 2:
            // 2. Delete contact
            {
                int del;
                cout<<"Enter 1 to delete by first name, 2 to delete by phone number: ";
                cin>>del;
                long int pho;
                string n;
                if(del == 1) // Delete using first name
                {
                    string word,line;
                    cout<<"Enter first name of person to delete: ";
                    cin>>n;
                    ifstream phone("phone.txt", ios::in);
                    if(!phone)
                    {
                        cout<<"file not opened properly"<<endl;
                        return -1;
                    }
                    // read each node from  file
                    while(!phone.eof())
                    {
                        phone>>word; // First word is first name
                        if(n == word)
                        {
                            phone>>word;
                            phone>>pho; // Read phone no.
                        }
                    }
                    phone.close();
                    curroot = delcontact(curroot,ph);
                }
                else // Delete using phone number -- default case
                {
                    cout<<"Enter phone no. to delete: ";
                    cin>>ph;
                    curroot = delcontact(curroot,ph);
                }
                cout<<"\nAfter deleting: Splay Tree:\n";
                preOrder(curroot);
                break;
            }
            case 3:
            // 3. Update contact in file
            {
                int upd;
                cout<<"Enter 1 to update first name, 2 to update phone no.: ";
                cin>>upd;
                long int ph,oldph;
                string n, oldn;
                if(upd == 1) // Update first name
                {
                    string word,line;
                    cout<<"Enter existing first name: ";
                    cin>>oldn;
                    cout<<"Enter new first name: ";
                    cin>>n;
                    fstream phone("phone.txt", ios::in|ios::out);
                    if(!phone)
                    {
                        cout<<"file not opened properly"<<endl;
                        return -1;
                    }
                    // read each node from  file
                    while(!phone.eof())
                    {
                        int pos = phone.tellg();
                        phone>>word; // First word is first name
                        if(n == word)
                        {
                            phone.seekg(pos,ios::beg);
                            phone<<n<<" ";
                        }
                    }
                    phone.close();
                    updfile(curroot,n,oldn);
                }
                else // Update phone number -- default case
                {
                    cout<<"Enter existing phone number: ";
                    cin>>oldph;
                    cout<<"Enter new phone no.: ";
                    cin>>ph;
                    updfile(curroot,ph,oldph);
                }
                curroot = NULL;
                string w1,w2; // Text file is space separated
                long int pho;
                node newn;
                ifstream phone("phone.txt", ios::in);
                if(phone.eof())
                {
                    cout<<"file not opened properly"<<endl;
                    return -1;
                }
                // read each node from  file
                while(!phone.eof())
                {
                    phone>>w1;
                    phone>>w2;
                    phone>>pho;
                    // Updating new node to insert into file
                    newn.setfn(w1);
                    newn.setln(w2);
                    newn.setph(pho);
                    curroot = insert(curroot,&newn);
                }
                phone.close();
                cout<<"Preorder traversal after updating: ";
                preOrder(curroot);
                break;
            }
            case 4:
            // 4. Search for a phone number
            {
                long start = gettime();
                long int phoneno;
                cout<<"Enter phone number to search for: ";
                cin>>phoneno;
                cout<<"Preorder traversal after searching for "<<phoneno<<": ";
                curroot = bstSearch(curroot, phoneno);
                long end = gettime();
                preOrder(curroot);
                splaysearch=(end-start);

                long start2 = gettime();
                node* s = avl.search(root,phoneno);
                long end2 = gettime();
                if(s == NULL)
                    cout<<"Does not exist in file."<<endl;
                else
                    cout<<"Node is present!"<<endl;
                avlsearch=(end2-start2)/10e5;
                break;
            }
            case 5:
            // 5. Recent Contacts List
            {
                cout<<"RECENTLY ACCESSED CONTACTS\n---------------------------------------------------\nNAME\t\t\t\tPHONE NUMBER\n---------------------------------------------------"<<endl;
                recentlist(curroot);
                break;
            }
            case 6:
            // 6. Alphabetic list using DLL
            {
                cout<<endl<<"\n----------Doubly Linked List:----------\n----------Alphabetic Order:----------";
                cout<<"\n---------------------------------------------------\nNAME\t\t\t\tPHONE NUMBER\n---------------------------------------------------"<<endl;
                print(head,1);
                cout<<endl;
                break;
            }
            case 7:
            // 7. Reverse (non-alphabetic) list using DLL
            {
                cout<<endl<<"\n----------Doubly Linked List:----------\n----------Non-increasing Order:----------";
                cout<<"\n---------------------------------------------------\nNAME\t\t\t\tPHONE NUMBER\n---------------------------------------------------"<<endl;
                print(head,0);
                cout<<endl;
                break;
            }
            case 8:
            // Constructed AVL tree from file
            {
                cout<<"\n----------Constructed AVL tree:----------\n";
                avl.display(root,1);
                cout<<endl;
                break;
            }
            case 9:
            // 9. Time taken by DLL, AVL and splay in inserting records from file
            {
                cout<<"Average time taken by insertion in DLL and splay tree:\n";
                cout<<"DLL\t\t\t\tAVL\t\t\t\tSplay Tree\n";
                cout<<llround((float)dll_ins/loop_times)<<" msec.s\t\t\t"<<llround((float)avl_ins/loop_times)<<" msec.s\t\t\t"<<llround((float)splay_ins/loop_times)<<" msec.s"<<endl;
                cout<<"Time taken for searching in AVL and splay tree:";
                cout<<"\nAVL\t\t\tSplay Tree\n";
                cout<<splaysearch<<" seconds\t\t"<<avlsearch<<" seconds\n";
                break;
            }
            case 10:
                cout<<"Exiting.."<<endl; exit(0);
            default:
                cout<<"Invalid choice. Exiting.."<<endl;
                exit(1);
        }
        loop_times++;
    }
    while(true);
    return 0;
}