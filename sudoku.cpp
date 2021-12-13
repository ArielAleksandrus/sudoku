// Sudoku.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

typedef struct Map {
	int** places;
	int size;
};
typedef struct Try {
	Map* base;
	Try* parent;
	Try** children;
	int children_count;
	bool status;
};
typedef struct Known {
	int* found;
	int qty;

	int* possibilities;
	int p_qty;

	int i_val, j_val;
};
Map* extractArgs(int, char**);
void printMap(Map*);
Map* copyMap(Map*);
bool resolve(Map*, Try*);
Known* findPossibilities(Map*, int, int);
void printPossibilities(Known*);
void addToKnown(Known*, int);


int iterations = 0;

int main(int argc, char** argv) {
	Map* m = extractArgs(argc, argv);
	printMap(m);

	resolve(m, NULL);

	return 0;
}

bool resolve(Map* m, Try* parent) {
	iterations++;

	int size = m->size;

	Try* t = (Try*)malloc(sizeof(Try));
	t->base = m;
	t->parent = parent;

	int i_val = -1, j_val = -1;

	// find nearest unknown (zeroed) value
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (t->base->places[i][j] == 0) {
				i_val = i;
				j_val = j;
				i = j = 100000; // break loops
			}
		}
	}

	if (i_val == -1 && j_val == -1) {
		cout << "Answer (after " << iterations << " iterations): " << endl;
		printMap(t->base);
		return true;
	}

	Known* k = findPossibilities(t->base, i_val, j_val);
	if (k->p_qty > 0) {
		t->children = (Try**)malloc(k->p_qty * sizeof(Try*));
		for (int i = 0; i < k->p_qty; i++) {
			Try* t2 = (Try*)malloc(sizeof(Try));
			t2->base = copyMap(m);
			t2->parent = t;

			t2->base->places[i_val][j_val] = k->possibilities[i];
			t->children[i] = t2;
			bool success = resolve(t2->base, t2);
			if (success) {
				return true;
			}
		}
		// printPossibilities(k);
	} else {
		// cout << "No possibilities for this iteration..." << endl;
		t->status = false;
	}
	free(k);

	return false;
}

Known* findPossibilities(Map* m, int row, int col) {
	Known* known = (Known*)malloc(sizeof(Known));
	known->found = (int*)malloc(m->size * sizeof(int));
	known->qty = 0;
	known->possibilities = (int*)malloc(m->size * sizeof(int));
	known->p_qty = 0;

	known->i_val = row;
	known->j_val = col;

	// fetch all possibilities for that row & col
	for (int k = 0; k < m->size; k++) {
		int val;

		val = m->places[row][k];
		addToKnown(known, val);

		val = m->places[k][col];
		addToKnown(known, val);
	}

	// fetch all possibilities for that square
	int square_size = sqrt(m->size);
	int square_i = row / square_size;
	int square_j = col / square_size;
	for (int i = 0; i < square_size; i++) {
		for (int j = 0; j < square_size; j++) {
			int val = m->places[square_i * square_size + i][square_j * square_size + j];
			addToKnown(known, val);
		}
	}

	for (int i = 1; i <= m->size; i++) {
		bool found = false;
		for (int j = 0; j < known->qty; j++) {
			if (known->found[j] == i) {
				found = true;
				j = 100000;
			}
		}
		if (!found) {
			known->possibilities[known->p_qty] = i;
			known->p_qty += 1;
		}
	}

	return known;
}
void printPossibilities(Known* k) {
	cout << "Possibilities for (" << k->i_val << ", " << k->j_val << "): ";
	for (int i = 0; i < k->p_qty; i++) {
		cout << k->possibilities[i] << " ";
	}
	cout << "." << endl;
}

void addToKnown(Known* known, int val) {
	if (val <= 0) {
		return;
	}

	bool found = false;
	for (int i = 0; i < known->qty; i++) {
		if (val == known->found[i]) {
			found = true;
		}
	}
	// val was not added yet. add now.
	if (!found) {
		known->found[known->qty] = val;
		known->qty += 1;
	}
}

Map* extractArgs(int argc, char** argv) {
	int size = argv[1][0] - '0';

	Map* m = (Map*)malloc(sizeof(Map));
	m->size = size;

	m->places = (int**)malloc(size * sizeof(int*));

	for (int i = 0; i < size; i++) {
		m->places[i] = (int*)malloc(size * sizeof(int));
	}

	for (int j = 2; j < argc; j++) {
		int place = argv[j][0] - '0';
		int row = (j - 2) / size;
		int col = (j - 2) % size;
		m->places[row][col] = place;
	}

	return m;
}

void printMap(Map* m) {
	int block_size = m->size;
	int block_dim = sqrt(block_size);

	for (int i = 0; i < block_size; i++) {
		if (i % block_dim == 0) {
			cout << "-------------------------" << endl;
		}
		cout << "| ";
		for (int j = 0; j < block_size; j++) {
			cout << m->places[i][j] << " ";
			if (j % block_dim == block_dim - 1) {
				cout << "| ";
			}
		}
		cout << endl;
	}
	cout << "-------------------------" << endl;
}

Map* copyMap(Map* in) {
	Map* m = (Map*)malloc(sizeof(Map));
	m->size = in->size;

	m->places = (int**)malloc(in->size * sizeof(int*));

	for (int i = 0; i < in->size; i++) {
		m->places[i] = (int*)malloc(in->size * sizeof(int));
		for (int j = 0; j < in->size; j++) {
			m->places[i][j] = in->places[i][j];
		}
	}

	return m;
}
