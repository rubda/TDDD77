

/* creates a new work set */
bool work_set_create(work_set* ws, int ws_max) {
    //TODO validate indata
    ws->data = malloc(ws_max*sizeof(int));
    ws->count = 0;
    return true;
}

/* adds an element to the set */
bool work_set_append(,work_set* ws, int val) {
    //TODO validate indata
    ws->data[ws->count] = val;
    ws->count++;
    return true;
}

/* removes an element from the set */
bool work_set_remove(work_set* ws, int val) {
    //TODO validate indata
    //maybe want to keep order of conditions, but for now: mess up order on remove!
    for (int i = 0; i < ws->count; i++) {
        if (ws->data[i] == val) {
            if (i == count-1)
            {
                ws->count--;
                return true;
            }
            else {
                ws->data[i] = ws->data[count-1];
                ws->count--;
                return true;
            }
        }
    }
    return false;
}

/* removes and deallocates the set */
bool work_set_free(work_set* ws) {
	free(ws->data);
    ws->data = NULL;
	free(ws);
    ws = NULL;
    return true;
}

bool work_set_print(work_set* ws) {
    for (int i = 0; i < count; i++) {
        printf("%d ,",ws->data[i]);
    }
    return true;
}