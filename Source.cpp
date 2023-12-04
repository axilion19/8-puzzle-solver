#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <list>
#define N 3
using namespace std;

int initialState[N][N] =
{
	{7, 2, 4},
	{5, 0, 6},
	{8, 3, 1}
};

int pos_X = 1, pos_Y = 1;

int goalState[N][N] =
{
	{2, 5, 0},
	{8, 7, 4},
	{3, 1, 6}
};

struct Node {
	int mat[N][N];
	Node* parent;
	int x;
	int y;
};

Node* newNode(int mat[N][N], int x, int y, Node* parent)
{
	Node* node = new Node;
	memcpy(node->mat, mat, sizeof node->mat);
	node->x = x;
	node->y = y;
	node->parent = parent;

	return node;
}

void move_Left(int state[N][N], int &pos_X, int &pos_Y) {
	if (pos_Y - 1 >= 0) {
		swap(state[pos_X][pos_Y - 1], state[pos_X][pos_Y]);
		pos_Y--;
	}
}

void move_Right(int state[N][N], int &pos_X, int &pos_Y) {
	if (pos_Y + 1 <= N) {
		swap(state[pos_X][pos_Y + 1], state[pos_X][pos_Y]);
		pos_Y++;
	}
}

void move_Up(int state[N][N], int &pos_X, int &pos_Y) {
	if (pos_X - 1 >= 0) {
		swap(state[pos_X - 1][pos_Y], state[pos_X][pos_Y]);
		pos_X--;
	}
}

void move_Down(int state[N][N], int& pos_X, int& pos_Y) {
	if (pos_X + 1 <= N) {
		swap(state[pos_X + 1][pos_Y], state[pos_X][pos_Y]);
		pos_X++;
	}
}

void swap(int& a, int& b) {
	int x = a;
	a = b;
	b = x;
}

bool is_Goal_State(int mat[N][N]) {
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) {
			if (mat[i][j] != goalState[i][j]) {
				return false;
			}
		}
	}
	return true;
}

void print_State(int mat[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << mat[i][j] << "  ";
		}
		cout << endl;
	}
}

bool is_Same_Mat(int mat1[N][N], int mat2[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) {
			if (mat1[i][j] != mat2[i][j]) {
				return false;
			}
		}
	}
	return true;
}
bool is_Expanded(Node* node, vector<Node*> v) {
	for (auto& i : v)
	{
		if (is_Same_Mat(node->mat, i->mat)) {
			return true;
		}
	}
	return false;
}

void print_Path(Node* n)
{
	stack <Node*> s;
	while (n->parent != NULL) {
		s.push(n);
		n = n->parent;
	}
	s.push(n);
	while (!s.empty()) {
		Node* temp;
		temp = s.top();
		print_State(temp->mat);
		cout << endl;
		s.pop();
	}
}



int h(int mat[N][N])
{
	int x = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (mat[i][j] == goalState[i][j]) {
				x++;
			}
		}
	}
	return x;
}

int misplaced_num(int mat[N][N])
{
	int x = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (mat[i][j] != goalState[i][j]) {
				x++;
			}
		}
	}
	return x;
}

int manhattan_distance(int mat[N][N])
{
	int f = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int temp = mat[i][j];
			for (int k = 0; k < N; k++)
			{
				for (int l = 0; l < N; l++)
				{
					if (goalState[k][l] == temp) {
						f += abs(i - k) + abs(j - l);
					}
				}
			}
		}
	}
	return f;
}

int path_Cost(Node* n)
{
	int x = 0;

	while (n->parent != NULL) {
		x++;
		n = n->parent;
	}
	return x;
}

void greedy()
{
	priority_queue <pair<int, Node*>> fringe;
	pair <int, Node*> p;

	vector<Node*> explored;
	int explored_count = 0;
	Node* root = newNode(initialState, pos_X, pos_Y, NULL);
	p.second = root;
	p.first = h(root->mat);
	fringe.push(p);
	cout << "Araniyor..." << endl;
	while (!fringe.empty())
	{
		Node* temp = fringe.top().second;

		if (is_Goal_State(temp->mat)) {
			cout << "Goal state founded!!0\n" << endl;
			cout << "\n Path: \n" << endl;
			print_Path(temp);
			cout << "\n\nExpanded nodes: " << explored_count << endl;
			return;
		}
		else {
			explored.push_back(temp);
			explored_count++;
			fringe.pop();
			if (temp->y > 0) {
				Node* x = newNode(temp->mat, temp->x, temp->y, temp);
				pair <int, Node* > p1;
				move_Left(x->mat, x->x, x->y);
				p1.second = x;
				p1.first = h(x->mat);
				if (is_Goal_State(x->mat)) {
					cout << "Goal state founded!!1\n" << endl;
					cout << "\n Path: \n" << endl;
					print_Path(temp);
					print_State(x->mat);
					cout << "\n\nExpanded nodes: " << explored_count << endl;
					return;
				}
				if (!is_Expanded(x, explored)) {
					fringe.push(p1);
				}
			}
			if (temp->x > 0) {
				Node* x = newNode(temp->mat, temp->x, temp->y, temp);
				pair <int, Node* > p1;
				move_Up(x->mat, x->x, x->y);
				p1.second = x;
				p1.first = h(x->mat);
				if (is_Goal_State(x->mat)) {
					cout << "Goal state founded!!2\n" << endl;
					cout << "\n Path: \n" << endl;
					print_Path(temp);
					print_State(x->mat);
					cout << "\n\nExpanded nodes: " << explored_count << endl;
					return;
				}
				if (!is_Expanded(x, explored)) {
					fringe.push(p1);
				}
			}
			if (temp->x < N - 1) {
				Node* x = newNode(temp->mat, temp->x, temp->y, temp);
				pair <int, Node* > p1;
				move_Down(x->mat, x->x, x->y);
				p1.second = x;
				p1.first = h(x->mat);
				if (is_Goal_State(x->mat)) {
					cout << "Goal state founded!!3\n" << endl;
					cout << "\n Path: \n" << endl;
					print_Path(temp);
					print_State(x->mat);
					cout << "\n\nExpanded nodes: " << explored_count << endl;
					return;
				}
				if (!is_Expanded(x, explored)) {
					fringe.push(p1);
				}
			}
			if (temp->y < N - 1) {
				Node* x = newNode(temp->mat, temp->x, temp->y, temp);
				pair <int, Node*> p1;
				move_Right(x->mat, x->x, x->y);
				p1.second = x;
				p1.first = h(x->mat);
				if (is_Goal_State(x->mat)) {
					cout << "Goal state founded!!4\n" << endl;
					cout << "\n Path: \n" << endl;
					print_Path(temp);
					print_State(x->mat);
					cout << "\n\nExpanded nodes: " << explored_count << endl;
					return;
				}
				if (!is_Expanded(x, explored)) {
					fringe.push(p1);
				}
			}
			
			if (!fringe.empty())
			{
				pair<int, Node*> best;
				best.first = fringe.top().first;
				best.second = fringe.top().second;
			    fringe.pop();
				fringe.push(best);
			}
		}
		
	}
}

void a_star()
{
	priority_queue <pair<int, Node*>> fringe;
	pair <int, Node*> p;

	vector<Node*> explored;
	int explored_count = 0;
	Node* root = newNode(initialState, pos_X, pos_Y, NULL);
	p.second = root;
	p.first = -(path_Cost(root) + manhattan_distance(root->mat));
	fringe.push(p);
	cout << "Araniyor..." << endl;
	while (!fringe.empty())
	{
		Node* temp = fringe.top().second;

		if (is_Goal_State(temp->mat)) {
			cout << "Goal state founded!!0\n" << endl;
			cout << "\n Path: \n" << endl;
			print_Path(temp);
			print_State(temp->mat);
			cout << "\n\nExplored node: " << explored_count << endl;
			return;
		}
		else {
			explored.push_back(temp);
			fringe.pop();
			explored_count++;
			if (temp->y > 0) {
				Node* x = newNode(temp->mat, temp->x, temp->y, temp);
				pair <int, Node* > p1;
				move_Left(x->mat, x->x, x->y);
				p1.second = x;
				p1.first = -(path_Cost(x) + manhattan_distance(x->mat));
				if (is_Goal_State(x->mat)) {
					cout << "Goal state founded!!1\n" << endl;
					cout << "\n Path: \n" << endl;
					print_Path(temp);
					print_State(x->mat);
					cout << "\n\nExplored node: " << explored_count << endl;
					return;
				}
				if (!is_Expanded(x, explored)) {
					fringe.push(p1);
				}
			}
			if (temp->x > 0) {
				Node* x = newNode(temp->mat, temp->x, temp->y, temp);
				pair <int, Node* > p1;
				move_Up(x->mat, x->x, x->y);
				p1.second = x;
				p1.first = -(path_Cost(x) + manhattan_distance(x->mat));
				if (is_Goal_State(x->mat)) {
					cout << "Goal state founded!!2\n" << endl;
					cout << "\n Path: \n" << endl;
					print_Path(temp);
					print_State(x->mat);
					cout << "\n\nExplored node: " << explored_count << endl;
					return;
				}
				if (!is_Expanded(x, explored)) {
					fringe.push(p1);
				}
			}
			if (temp->x < N - 1) {
				Node* x = newNode(temp->mat, temp->x, temp->y, temp);
				pair <int, Node* > p1;
				move_Down(x->mat, x->x, x->y);
				p1.second = x;
				p1.first = -(path_Cost(x) + manhattan_distance(x->mat));
				if (is_Goal_State(x->mat)) {
					cout << "Goal state founded!!3\n" << endl;
					cout << "\n Path: \n" << endl;
					print_Path(temp);
					print_State(x->mat);
					cout << "\n\nExplored node: " << explored_count << endl;
					return;
				}
				if (!is_Expanded(x, explored)) {
					fringe.push(p1);
				}
			}
			if (temp->y < N - 1) {
				Node* x = newNode(temp->mat, temp->x, temp->y, temp);
				pair <int, Node*> p1;
				move_Right(x->mat, x->x, x->y);
				p1.second = x;
				p1.first = -(path_Cost(x) + manhattan_distance(x->mat));
				if (is_Goal_State(x->mat)) {
					cout << "Goal state founded!!4\n" << endl;
					cout << "\n Path: \n" << endl;
					print_Path(temp);
					print_State(x->mat);
					cout << "\n\nExplored node: " << explored_count << endl;
					return;
				}
				if (!is_Expanded(x, explored)) {
					fringe.push(p1);
				}
			}

			if (!fringe.empty())
			{
				pair<int, Node*> best;
				best.first = fringe.top().first;
				best.second = fringe.top().second;
				fringe.pop();
				fringe.push(best);
			}
		}

	}
}

int main()
{
	greedy();

	//a_star();
}