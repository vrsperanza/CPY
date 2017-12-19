void selectionSort(int * arr, int n)
	for i = 0; i < n; i++
		for j = i+1; j < n; j++
			if arr[i] > arr[j]
				aux = arr[i]
				arr[i] = arr[j]
				arr[j] = aux