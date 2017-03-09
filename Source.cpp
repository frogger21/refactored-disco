#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

/******************************************
	MEDIAN MAINTENACE WITH HEAPS
******************************************/

//*************************
// CUSTOM HEAP: J_HEAP
//*************************

template <typename T2>
bool comparemax(T2 &A, T2 &B) {
	return A < B;  //max heap
}

template <typename T22>
bool comparemin(T22 &A, T22 &B) {
	return A > B;  //min heap
}

template <typename T4, typename Pred4>
void j_heapify(vector<T4> &p, int N, Pred4 predicate) {
	for (int k = N / 2; k >= 1; k--) {
		j_sinkheap(p, N, k, predicate);
	}
}

template <typename T1, typename Pred1>
void j_swimheap(vector<T1> &p, int N, int k, Pred1 predicate) {
	T1 temp;
	while (k > 1 && (predicate(p[k / 2], p[k]))) {
		temp = p[k];
		p[k] = p[k / 2];
		p[k / 2] = temp;
		k = k / 2;
	}
}

template <typename T3, typename Pred2>
void j_sinkheap(vector<T3> &p, int N, int k, Pred2 predicate) {
	T3 temp;
	while (2 * k <= N) {
		int j = 2 * k; //the child & j++ its other child
		if (j < N && predicate(p[j], p[j + 1])) {
			j += 1;
		} //we have chosen our favourite child
		if (predicate(p[k], p[j]) == false) {
			break; //parent dominates favourite child nothing to do
		}
		else {
			//child dominates the parent so swap
			temp = p[j];
			p[j] = p[k];
			p[k] = temp;
			k = j;
		}
	}
}

//*************************
// FUNCTIONS f_functionName
//*************************
//parses string line and looks for numbers to put into a list
//ignores all non number ascii
void f_split(string line, vector<double> &v) {
	string::size_type sz;
	int NumCount = 0;
	int startPos, numLen, tracker, StringSize;
	double theNumber;
	StringSize = line.size();
	bool Num = false;
	bool temp = false;
	bool temp2 = false;
	for (int i = 0; i < StringSize; i++) {
		//is it a number?
		temp = false;
		if (line[i] >= 48 && line[i] <= 57) {
			temp = true;
		}
		//if it's not a number then 
		if (temp == false) {
			Num = false;
		}
		//previous char was not a number but current is: we got a new number
		if (Num == false && temp == true) {
			Num = true;
			startPos = i;
			numLen = 1; //we don't know yet fully
			tracker = i + 1;
			while (tracker < StringSize) {
				temp2 = false;
				if (line[tracker] <= 57 && line[tracker] >= 48) {
					temp2 = true;
				}
				if (temp2 == true) {
					numLen += 1;
					tracker++;
				}
				else {
					// don't need to check these anymore cuz we already know 
					//it's # -1 b/c we i++ in for loop
					i += numLen - 1;
					break;
				}
			}
			string test2 = line.substr(startPos, numLen);
			theNumber = stod(test2, &sz); //convert string to long
			NumCount += 1;
			v.push_back(theNumber);
		}
	}
}

void f_printHeap(vector<double> &vecF) {
	for (int i = 0; i < vecF.size(); i++) {
		cout << vecF[i] << ", ";
	}
	cout << " END\n";
}

int main() {
	//reading file to input into a vector
	const string str_readfile = "D:/Median.txt";
	vector<double> vec_storeinputs;
	ifstream std_inputFile;
	std_inputFile.open(str_readfile);
	if (std_inputFile.is_open()) {
		string str_line;
		while (getline(std_inputFile, str_line)) {
			f_split(str_line, vec_storeinputs);
		}
	}

	//just visual check if vector holds proper elements
	if (1 == 3) {
		for (int i = 0; i < vec_storeinputs.size(); i++) {
			std::cout << vec_storeinputs[i] << endl;
		}
	}

	//******************************
	//the STAR of the show
	//median maintenance with heaps
	//******************************
	vector<double> vec_minheap; //anything >= effective median goes here
	vector<double> vec_maxheap; //anything < effective median goes here
	vector<double> vec_median; //store the median here
	
	//my heaps count 1 to N so push 0 with nulls
	vec_minheap.push_back(NULL);
	vec_maxheap.push_back(NULL);
	int int_count = 0;
	int int_minSize, int_maxSize;
	double dbl_sum = 0.0;
	double dbl_temp;
	double dbl_median = 0.0; //initialize median
	bool b_avg = 0; //0 = don't avg median, 1 = avg the median i.e. even numbers 1 2 3 4, median is 2.5. while 0 option is 2
	
	//start streaming the numbers
	for (int i = 0; i < vec_storeinputs.size(); i++) {
		int_count += 1;
		int_minSize = vec_minheap.size()-1; //b/c we push_back(NULL)
		int_maxSize = vec_maxheap.size()-1; //ditto
		
		//3 cases, minSize < maxSize, minSize == maxSize, minSize > maxSize
		if (int_minSize == int_maxSize) {
			//the trees are already balanced
			if (vec_storeinputs[i] >= dbl_median) {
				vec_minheap.push_back(vec_storeinputs[i]);
				j_swimheap(vec_minheap, int_minSize + 1, int_minSize+1, comparemin<double>);
				dbl_median = vec_minheap[1];
			}
			else {
				vec_maxheap.push_back(vec_storeinputs[i]);
				j_swimheap(vec_maxheap, int_maxSize + 1, int_maxSize+1, comparemax<double>);
				dbl_median = vec_maxheap[1];
			}
			//now one heap is bigger than the other heap by exactly 1 element
			
			/*
			cout << "L: ";
			f_printHeap(vec_maxheap);
			cout << "R: ";
			f_printHeap(vec_minheap);
			*/
		} 
		else if(int_minSize > int_maxSize) {
			//minHeap bigger than maxHeap
			if (vec_storeinputs[i] >= dbl_median) {
				//need to add to minheap but it's already bigger so balance the trees
				vec_maxheap.push_back(vec_minheap[1]);
				j_swimheap(vec_maxheap, int_maxSize + 1, int_maxSize + 1, comparemax<double>);
				vec_minheap[1] = vec_minheap[int_minSize];
				j_sinkheap(vec_minheap, int_minSize - 1, 1, comparemin<double>);
				vec_minheap[int_minSize] = vec_storeinputs[i];
				j_swimheap(vec_minheap, int_minSize, int_minSize, comparemin<double>);
			}
			else {
				//need to add to maxheap
				vec_maxheap.push_back(vec_storeinputs[i]);
				j_swimheap(vec_maxheap, int_maxSize + 1, int_maxSize + 1, comparemax<double>);
			}

			//now they both have same # of elements
			//grab the new effective median
			if (b_avg == 1) {
				dbl_median = (vec_minheap[1] + vec_maxheap[1]) / 2.0;
			}
			else {
				if (vec_minheap[1] <= vec_maxheap[1]) {
					dbl_median = vec_minheap[1];
				}
				else {
					dbl_median = vec_maxheap[1];
				}
			}
			
			/*
			cout << "L: ";
			f_printHeap(vec_maxheap);
			cout << "R: ";
			f_printHeap(vec_minheap);
			*/
		}
		else if (int_minSize < int_maxSize) {
			//minHeap smaller than maxHeap
			if (vec_storeinputs[i] >= dbl_median) {
				//we need to add this to minheap
				vec_minheap.push_back(vec_storeinputs[i]);
				j_swimheap(vec_minheap, int_minSize + 1, int_minSize + 1, comparemin<double>);
			}
			else {
				//we need to add this to maxheap but it's already bigger so take out
				//top element from it and insert it into the minheap
				vec_minheap.push_back(vec_maxheap[1]);
				j_swimheap(vec_minheap, int_minSize + 1, 1+int_minSize, comparemin<double>);
				vec_maxheap[1] = vec_maxheap[int_maxSize];
				j_sinkheap(vec_maxheap, int_maxSize - 1, 1, comparemax<double>);
				vec_maxheap[int_maxSize] = vec_storeinputs[i];
				j_swimheap(vec_maxheap, int_maxSize, int_maxSize, comparemax<double>);
			}

			//now they both have same # of elements
			//grab the new effective median
			if (b_avg == 1) {
				dbl_median = (vec_minheap[1] + vec_maxheap[1]) / 2.0;
			}
			else {
				if (vec_minheap[1] <= vec_maxheap[1]) {
					dbl_median = vec_minheap[1];
				}
				else {
					dbl_median = vec_maxheap[1];
				}
			}
			
			/*
			cout << "L: ";
			f_printHeap(vec_maxheap);
			cout << "R: ";
			f_printHeap(vec_minheap);
			*/
		} //end of if
		vec_median.push_back(dbl_median);
		std::cout << int_count << " : " <<  vec_median[int_count - 1] << endl;
		dbl_temp = vec_median[int_count - 1];
		dbl_sum += dbl_temp;
	} //end of for
	cout << "SUM: " << (int)dbl_sum%10000 << endl;


	//fin
	system("PAUSE");
	return 0;
}
