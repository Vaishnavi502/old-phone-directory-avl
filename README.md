# AVL Trees run on Phone Directory system
<br>
Uses C++<br>
This project implements a phone's contact directory (a listing of telephone numbers) using doubly linked lists, splay and AVL trees.
<br>
The project will demonstrate searching for a particular telephone number, finding and deleting duplicates (when inserting into a splay tree), adding new telephone numbers and updating any given number. The main aim of this project is to create an optimized phone directory system. Additionally, it shows the most recently used contact(s) through splay tree traversal and listing of contacts in alphabetic and reverse order.
<br>
This application focuses on CRUD operations for a phone's contact directory. Faster searching and avoiding redundancy are the most beneficial features for a user. 
Data stored in the doubly linked list nodes will be a contact's first, last names and phone number (of 10 digits). Data is read from the existing csv file (text separated) and stored in a splay tree. Doubly linked lists and AVL trees are constructed simultaneously, showing all changes after each operation. New input is appended to the file.
Since a phone contact list is in sorted order, and we need optimized searching implementation, the existing DLL is sorted using a merge sort algorithm.
To compare the insertions in DLL, AVL tree and splay tree, we use time.h header file to find the time taken in each function and display comparison.
<br>
Data file:<br>
![image](https://github.com/Vaishnavi502/old-phone-directory-avl/assets/68768878/1671bcab-493e-4d3d-80f8-6e29d7e8e39b)
