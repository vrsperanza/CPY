
//For now, pointers should always stick to the variable name
//This way, int is the type and *arr is the name, thus this expands to:
//void sort(int *arr, int n)
void selectionSort(int *arr, n)
	for i = 0; i < n; i++
		for j = i+1; j < n; j++
			if arr[i] > arr[j]
				aux = arr[i]
				arr[i] = arr[j]
				arr[j] = aux