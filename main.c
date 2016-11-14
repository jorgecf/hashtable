#include "includes/hashtable.h"

int main () {

	hashtable *ht = ht_create(50);

	if (!ht) {
		fprintf(stderr, "Error creating hash table...");
		exit(EXIT_FAILURE);
	}


	ht_value v, w;
	v.v = 123;
	w.v = 789;

	int ret = ht_insert(ht, "test", &v);
	if (ret < 0) {
		fprintf(stderr, "error inserting key...: %d\n", ret);
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "Inserted test key, value %d.\n", v.v);


	if (ht_keyexists(ht, "test") == HT_KEYEXISTS) {
		fprintf(stdout, "Key test does exist.\n");
		fprintf(stdout, "Value for test key is %d.\n", ht_getvalue(ht, "test")->v);
	} else {
		fprintf(stdout, "Key test does not exist.\n");
	}


	ht_insert(ht, "testagain", &v);

//	ht_print(stdout, ht);

	ht_deletenode(ht, "test");

	ht_fill(ht, "ABC", &v, "XYZ", &w);

	ht_print(stdout, ht);

	ht_deletenode(ht, "ABC");

	ht_print(stdout, ht);

	ht_clear(ht);
}
