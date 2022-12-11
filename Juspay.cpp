#include <bits/stdc++.h>
using namespace std;

struct Node
{
    string v;
    vector<Node *> children;
    Node *parentNode;
    int ancestor, decendant, userid;
    bool lockBool;

    Node(string x, Node *p)
    {
        v = x;
        parentNode = p;
        ancestor = 0;
        decendant = 0;
        userid = 0;
        lockBool = false;
    }

    void addEdges(vector<string> l, Node *p)
    {
        for (auto &i : l)
            children.push_back(new Node(i, p));
    }
};

struct Node *makeTree(Node *head, int &m, vector<string> &s)
{
    queue<Node *> breadthFS;
    breadthFS.push(head);

    int st = 1;
    while (!breadthFS.empty())
    {
        Node *r = breadthFS.front();
        breadthFS.pop();

        if (st >= s.size())
            continue;

        vector<string> temp;
        for (int i = st; i < st + m; i++)
            temp.push_back(s[i]);
        r->addEdges(temp, r);
        st += m;

        for (auto k : r->children)
            breadthFS.push(k);
    }

    return head;
}

class Tree
{
private:
    Node *head;
    unordered_map<string, Node *> nameToNodeMap;

public:
    Tree(Node *r) { head = r; }

    Node *gethead() { return head; }

    void makeMap(Node *r)
    {
        if (!r)
            return;
        nameToNodeMap[r->v] = r;
        for (auto k : r->children)
            makeMap(k);
    }

    void tellDecendants(Node *r, int val)
    {
        for (auto k : r->children)
        {
            k->ancestor += val;
            tellDecendants(k, val);
        }
    }

    bool checkChildren(Node *r, int &id, vector<Node *> &v)
    {
        if (r->lockBool)
        {
            if (r->userid != id)
                return false;
            v.push_back(r);
        }
        if (r->decendant == 0)
            return true;

        bool ans = true;
        for (auto k : r->children)
        {
            ans &= checkChildren(k, id, v);
            if (ans == false)
                return false;
        }
        return ans;
    }

    bool lock(string v, int id)
    {
        Node *t = nameToNodeMap[v];
        if (t->lockBool)
            return false;

        if (t->ancestor != 0)
            return false;
        if (t->decendant != 0)
            return false;

        Node *cur = t->parentNode;
        while (cur)
        {
            cur->decendant++;
            cur = cur->parentNode;
        }
        tellDecendants(t, 1);
        t->lockBool = true;
        t->userid = id;
        return true;
    }

    bool unlock(string v, int id)
    {
        Node *t = nameToNodeMap[v];
        if (!t->lockBool)
            return false;
        if (t->lockBool && t->userid != id)
            return false;

        Node *cur = t->parentNode;
        while (cur)
        {
            cur->decendant--;
            cur = cur->parentNode;
        }
        tellDecendants(t, -1);
        t->lockBool = false;
        return true;
    }

    bool upgradeFunction(string v, int id)
    {
        Node *t = nameToNodeMap[v];
        if (t->lockBool)
            return false;

        if (t->ancestor != 0)
            return false;
        if (t->decendant == 0)
            return false;

        vector<Node *> vec;
        if (checkChildren(t, id, vec))
        {
            for (auto k : vec)
            {
                unlock(k->v, id);
            }
        }
        else
            return false;
        lock(v, id);
        return true;
    }
};

int main()
{
    int n, m, q;
    cin >> n;
    cin >> m;
    cin >> q;

    vector<string> s(n);
    for (int i = 0; i < n; i++)
        cin >> s[i];

    Node *r = new Node(s[0], nullptr);
    r = makeTree(r, m, s);
    // printTree(r);

    Tree t(r);
    t.makeMap(t.gethead());

    int op, userid;
    string sq;
    for (int i = 0; i < q; i++)
    {
        cin >> op >> sq >> userid;
        switch (op)
        {
        case 1:
            if (t.lock(sq, userid))
            {
                cout << "true\n";
                // printTree(r);
            }
            else
                cout << "false\n";
            break;
        case 2:
            if (t.unlock(sq, userid))
                cout << "true\n";
            else
                cout << "false\n";
            break;
        case 3:
            if (t.upgradeFunction(sq, userid))
            {
                cout << "true\n";
                // printTree(r);
            }
            else
                cout << "false\n";
            break;
        }
    }
    return 0;
}