#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 1025

struct node
{
	char name[MAX];
	struct node* left;
	struct node* right;
};
typedef struct node node_t;

//linked list to store the parent name with tab count
struct list{
	int tab_count;
	node_t* data;
	struct list* next;
};
typedef struct list l_list;

struct tree
{
	node_t *root;
};
typedef struct tree tree_t;

void make_tree(tree_t *tree);
void disp_tree(const tree_t *ptr_tree);
node_t* init_node(char *name);
void push(l_list** head_ref, node_t* new_data,int tab_count);
void gen_case(int i,const char* s);
void end_while();
void gen_while();
void prolog();
void gen_header();
void epilog();
void generate_conditions(const node_t* node,int n);
void generate_file(tree_t *t);
void free_list(l_list* head);
void free_tree(node_t *node);

int main(){

	//generate the tree
	tree_t t; 	
	make_tree(&t);
	//generate the c file from tree
	generate_file(&t);
	// disp_tree(&t);
	//free tree
	free_tree(t.root);
	return 0;
}
void generate_file(tree_t *t){
	//generates the entire c file
	gen_header();
	prolog();
	printf("int choice;\n");
	gen_while();
	generate_conditions(t->root,1);
	//the first while loop should not have a break
	printf("}scanf(\"%%d\",&choice);}\n");
	epilog();
}

void generate_conditions(const node_t* node,int n){
	//generate a case
	gen_case(n,node->name);
	if(node->left){
		//generates a new while with a switch case for child
		gen_while();
		generate_conditions(node->left,1);
		end_while();

	}
	//adds a break before the next case
	printf("break;\n" );
	if(node->right){
		generate_conditions(node->right,++n);
		
	}
	
}

void make_tree(tree_t *t){
	t->root=NULL;
	char line[MAX]; int tab_count=0; 
	char str_temp[MAX]; int depth=0; 
	//the memset sets the entire string to \0
	memset(str_temp, '\0', sizeof str_temp);
	fgets(line, MAX, stdin);
	//this strips the fgets input of \n
	line[strcspn(line,"\n")]=0;
	strncpy(str_temp,line,strlen(line));
	node_t* new = init_node(str_temp);
	node_t* prev = new;
	t->root=new;
	
	l_list* head = NULL;
	l_list* index = NULL;
	
	while(fgets(line, MAX, stdin) != NULL)
	{

		tab_count=0;
		memset(str_temp, '\0', sizeof str_temp);
		while(line[tab_count]=='\t')
			tab_count++;
		line[strcspn(line,"\n")]=0;
		//copies the input after striping the tabs into str_temp
		strncpy(str_temp,line + tab_count,strlen(line) - tab_count);
		
		new = init_node(str_temp);
		
		

		if(tab_count > depth){
			//this is for a child. It also adds prev to linked list  
			push(&head,prev,tab_count-1);
			
			prev->left=new;
			prev=prev->left;

		}
		else if(tab_count == depth){
			//If the new node is a sibling to the previous node, add to right
			prev->right=new;
			prev=prev->right;

		}
		else if(tab_count < depth){
			//for a node to be added to previous node
			index=head;
			while (index != NULL) 
  			{ 
  				//finds the proper sibling in the linked list
   				if(tab_count==index->tab_count){
   					prev=index->data;
   					//adds the new node to the rightmost of the sibling
  					while(prev->right!=NULL)
  						prev=prev->right;
  					prev->right=new;
  					prev=prev->right;
  				
   					break;
   				} 
   				index=index->next;
  			}
  						
		}
		//stores the previous tab_count 
		depth=tab_count;
	}
	free_list(head);
}
node_t* init_node(char *name){
	node_t* node = (node_t*)malloc(sizeof(node_t));
	strcpy(node->name,name);
	node->left = node->right = NULL;
	return node;
}
void free_list(l_list* head)
{
   l_list* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }

}
void free_tree(node_t *node)
{
	// goes to the leaves, then frees recursively to the top
	if(node->left)
		free_tree(node->left);
	if(node->right)
		free_tree(node->right);
	free(node);
}
void disp_r(const node_t *node)
{
	//prints the nodes from top to bottm. A nodes children are printed before siblings
	printf("%s\n",node->name);
	if(node->left)
		disp_r(node->left);
	if(node->right)
		disp_r(node->right);
}



void disp_tree(const tree_t *ptr_tree)
{
	printf("\n");
	disp_r(ptr_tree -> root);
}
void push(l_list** head_ref, node_t* new_data,int tab_count) 
{  
	//inserts the new parent into the start of the list
    l_list* new_node = (l_list*) malloc(sizeof(l_list)); 
    new_node->data  = new_data;
    new_node->tab_count = tab_count; 
    new_node->next = (*head_ref); 
    (*head_ref) = new_node; 
} 

void gen_header(){
	printf("#include <stdio.h>\n");
}
void prolog(){
	printf("int main()\n");
	printf("{\n");
}
void epilog(){
	printf("}\n");
}
void gen_while(){
	
	printf("scanf(\"%%d\",&choice);\n");
	printf("while(choice!=0){\n");
	printf("switch(choice){\n" );
}
void end_while(){
	// printf("break;\n" );
	printf("}scanf(\"%%d\",&choice);}\n");
}

void gen_case(int i,const char* s){
	printf("case %d: \n",i);
	printf("printf(\"%s\\n\");\n",s);
}
