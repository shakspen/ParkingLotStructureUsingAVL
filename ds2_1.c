#include<stdio.h>
#include<stdlib.h>

//Details are Stored in AVL tree

struct f_details_node
{
	int ID;
	int V_no;	// 0 if no vehicle
	int wing; // 1 for A , 2 for B , 3 for C
	int V_type; // 0 for no vehicle , 1 for 2-wheeler , 2 for 4-wheeler
	struct f_details_node * left;
	struct f_details_node * right;
	int height;
};

struct parking_details_node
{
	int P_level;	// 1 for wing A ; 2 for Wing B ; 3 for Wing C
	int spot_no;
	int V_no ;		// 0 if no vehicle is mapped with this spot
	int spot; 		//1 for small ; 2 for large 
	int status;		// 0 for not parked , 1 for parked
	struct parking_details_node * left;
	struct parking_details_node * right;
	int height;
};

int height_f(struct f_details_node * N)
{
	if(N==NULL)
	{
		return 0;
	}
	return N->height;
}

int height_p(struct parking_details_node * N)
{
	if(N==NULL)
	{
		return 0;
	}
	return N->height;
}

int max(int a, int b) 
{ 
    return (a > b)? a : b; 
} 

struct f_details_node * newNode_f(int id , int Vno , int vtype)
{
	struct f_details_node * node = (struct f_details_node *)malloc(sizeof(struct f_details_node));
	node -> ID = id;
	node -> V_no = Vno;
	node -> V_type = vtype;
	if((id>=1)&&(id<=36))
	{
		node -> wing = 1;
	}
	else if((id>36)&&(id<73))
	{
		node -> wing = 2;
	}
	else
	{
		node -> wing = 3;
	}
	
	node -> height = 1;	//node is initially added at leaf
	
	node -> left = NULL;
	node -> right = NULL;
	node -> height = 1;
	
	return(node);
}

struct parking_details_node * newNode_p(int s_no , int vno , int spot)
{
	struct parking_details_node * node = (struct parking_details_node *)malloc(sizeof(struct parking_details_node));
	
	node -> spot_no = s_no;
	if((s_no>0)&&(s_no<37))
	{
		node -> P_level = 1;
	}
	else if((s_no>36)&&(s_no<73))
	{
		node -> P_level = 2;
	}
	else
	{
		node -> P_level = 3;
	}
	
	node -> V_no = vno;
	node -> spot = spot;
	node -> status = 0;
	
	node -> left = NULL;
	node -> right = NULL;
	node -> height = 1;
	
	return(node);
}

struct f_details_node * rightRotate_f(struct f_details_node * y)
{
	struct f_details_node * x = y -> left;
	struct f_details_node * T2 = x -> right;
	
	x->right = y;
	y->left = T2;
	
	y -> height = max(height_f(y->left), height_f(y->right))+1;
	x -> height = max(height_f(x->left), height_f(x->right))+1;
	
	return x;
}

struct parking_details_node * rightRotate_p(struct parking_details_node * y)
{
	struct parking_details_node * x = y -> left;
	struct parking_details_node * T2 = x -> right;
	
	x->right = y;
	y->left = T2;
	
	y -> height = max(height_p(y->left), height_p(y->right))+1;
	x -> height = max(height_p(x->left), height_p(x->right))+1;
	
	return x;
}

struct f_details_node * leftRotate_f(struct f_details_node * x)
{
	struct f_details_node * y = x -> right;
	struct f_details_node * T2 = y -> left;
	
	y -> left = x;
	x -> right = T2;
	
	x -> height = max(height_f(x->left), height_f(x->right))+1;
	y -> height = max(height_f(y->left), height_f(y->right))+1;
	
	return y;
}

struct parking_details_node * leftRotate_p(struct parking_details_node * x)
{
	struct parking_details_node * y = x -> right;
	struct parking_details_node * T2 = y -> left;
	
	y -> left = x;
	x -> right = T2;
	
	x -> height = max(height_p(x->left), height_p(x->right))+1;
	y -> height = max(height_p(y->left), height_p(y->right))+1;
	
	return y;
}
int getBalance_f(struct f_details_node * N)
{
	if (N == NULL)
		return 0;
		
	return height_f(N->left) - height_f(N->right);
}

int getBalance_p(struct parking_details_node * N)
{
	if (N == NULL)
		return 0;
		
	return height_p(N->left) - height_p(N->right);
}

struct f_details_node * insert_f(struct f_details_node * node , int id , int Vno , int vtype)
{
	if(node == NULL)
		return(newNode_f(id,Vno,vtype));
		
	if(id < node->ID)
		node -> left = insert_f(node->left,id,Vno,vtype);
	else if(id > node->ID)
		node -> right = insert_f(node->right,id,Vno,vtype);
	else
		return node;
		
	node->height = 1 + max(height_f(node->left), height_f(node->right));
	
	int balance = getBalance_f(node);
	
	if((balance > 1) && (id < node->left->ID))
		return rightRotate_f(node);
		
	if((balance < -1) && (id > node->right->ID))
		return leftRotate_f(node);
	
	if((balance > 1) && (id > node->left->ID))
	{
		node -> left = leftRotate_f(node->left);
		return rightRotate_f(node);
	}
	
	if((balance < -1) && (id < node->right->ID))
	{
		node -> right = rightRotate_f(node -> right);
		return leftRotate_f(node);
	}
	
	return node;
}

struct parking_details_node * insert_p(struct parking_details_node * node , int s_no , int vno , int spot)
{
	if(node == NULL)
	{
		return (newNode_p(s_no,vno,spot));
	}
	if(s_no < node->spot_no)
		node -> left = insert_p(node->left,s_no,vno,spot);
	else if(s_no > node->spot_no)
		node->right = insert_p(node->right,s_no,vno,spot);
	else
		return node;
		
	node->height = 1 + max(height_p(node->left), height_p(node->right));
	
	int balance = getBalance_p(node);
	
	if((balance > 1) && (s_no < node->left->spot_no))
		return rightRotate_p(node);
		
	if((balance<-1) && (s_no > node->right->spot_no))
		return leftRotate_p(node);
		
	if((balance > 1) && (s_no > node->left->spot_no))
	{
		node->left = leftRotate_p(node->left);
		return rightRotate_p(node);
	}	
		
	if((balance < -1) && (s_no < node->right->spot_no))
	{
		node->right = rightRotate_p(node->right);
		return leftRotate_p(node);
	}
	
	return node;
}

void add_map_vehicle(struct f_details_node * node, int id, int vtype)
{
	if(node!=NULL)
	{
		if(node->ID == id)
		{
			if(node->V_no!=0)
			{
				printf("You already had a Vehicle");
			}
			else
			{
				
				node->V_no = id;
				node->V_type = vtype;
			}
		}
		else if(id < node->ID)
		{
			add_map_vehicle(node->left, id, vtype);
		}
		else
		{
			add_map_vehicle(node->right, id ,vtype);
		}
	}
}

void add_map_v_p(struct parking_details_node * node, int id)
{
	if(node!=NULL)
	{
		if(node->spot_no == id)
		{
			node->V_no = id;
		}
		else if(id < node->spot_no)
		{
			add_map_v_p(node->left, id);
		}
		else
		{
			add_map_v_p(node->right, id);
		}
	}
}

void park_vehicle(struct parking_details_node * node, int sno)
{
	if(node!=NULL)
	{
		if(node->spot_no == sno)
		{
			node->status = 1;
		}
		else if(sno < node->spot_no)
		{
			park_vehicle(node->left, sno);
		}
		else
		{
			park_vehicle(node->right, sno);
		}
	}
}



void display_visitors(struct parking_details_node * node)
{
	if(node!=NULL)
	{
		display_visitors(node->left);
		
		
		
		printf("\n---------------------------------\n");
		printf("\nParking Level : %d",node->P_level);
		if(node->V_no != 0)
		{
			printf("\nVehicle No. : %d ",node->V_no);
			
			if(node->spot == 1)
			{	
				printf("\nVehicle Type : Two Wheeler");
			}
			else 
			{
				printf("\nVehicle Type : four Wheeler");
			}
		}
		else
		{
			printf("\nNo vehicle");
		}	
		
		
		display_visitors(node->right);	
	}
}

void parking_status(struct parking_details_node * node)
{
	if(node!=NULL)
	{
		parking_status(node->left);
		
		
		if(node->V_no == 0)
		{
			printf("\n*************************************\n");
			printf("spot no. %d is free ",node->spot_no);
		//	printf("*************************************\n");
		}
		if(node->status == 1)
		{
			printf("\n*************************************\n");
			printf("Vehicle on spot no. %d is parked",node->spot_no);
		//	printf("\n*************************************");
		}
			
		
		parking_status(node->right);		
	}
}

void Remove_Vehicle_node(struct f_details_node * node, int id)
{
	if(node!=NULL)
	{
		if(node->ID == id)
		{
			node->V_no = 0;
			node->V_type=0;
		}
		else if(id < node->ID)
		{
			Remove_Vehicle_node(node->left, id);
		}
		else
		{
			Remove_Vehicle_node(node->right, id);
		}
	}
}

void Remove_v_p(struct parking_details_node * node, int sno)
{
	if(node!=NULL)
	{
		if(node->spot_no == sno)
		{
			node->V_no = 0;
		}
		else if(sno < node->spot_no)
		{
			Remove_v_p(node->left, sno);
		}
		else
		{
			Remove_v_p(node->right, sno);	
		}
	}
}

int main()
{
	struct f_details_node * root_f = NULL;
	struct parking_details_node * root_p = NULL;
	int i=0;
	
	//-->		Hardcoding of Flat details			<--//

	for(i=1;i<10;i++)
	{
		root_f = insert_f(root_f, i+1, i+1, 1);
	}
	root_f = insert_f(root_f, 11, 0, 0);
	root_f = insert_f(root_f, 12, 0, 0);
	for(i=13;i<19;i++)
	{
			root_f = insert_f(root_f, i, i, 2);
	}
	for(i=19;i<27;i++)
	{
		root_f = insert_f(root_f, i, i, 1);
	}
	for(i=27;i<34;i++)
	{
		root_f = insert_f(root_f, i, 0, 0);
	}
	for(i=34;i<=36;i++)
	{
		root_f = insert_f(root_f, i, i, 2);
	}

	
	for(i=37;i<=45;i++)
	{
		root_f = insert_f(root_f, i, i, 1);
	}
	for(i=46;i<=50;i++)
	{
		root_f = insert_f(root_f, i, 0, 0);
	}
	for(i=51;i<=64;i++)
	{
		root_f = insert_f(root_f, i, i, 2);
	}
	for(i=65;i<=72;i++)
	{
		root_f = insert_f(root_f, i, i, 1);
	}
	
	
	for(i=73;i<=81;i++)
	{
		root_f = insert_f(root_f, i, i, 1);
	}
	for(i=82;i<=94;i++)
	{
		root_f = insert_f(root_f, i, i, 2);
	}
	for(i=95;i<=100;i++)
	{
		root_f = insert_f(root_f, i, 0, 0);
	}
	for(i=101;i<=108;i++)
	{
		root_f = insert_f(root_f, i, i, 1);
	}
	
	//--> 		end of flat details				<--//

	
	//-->		Hardcoding of parking details	<--//
	for(i=1;i<=10;i++)
	{
		root_p = insert_p(root_p, i, i, 1);
	}
	root_p = insert_p(root_p, 11, 0, 1);
	root_p = insert_p(root_p, 12, 0, 1);
	for(i=13;i<=18;i++)
	{
		root_p = insert_p(root_p, i, i, 2);
	}
	for(i=19;i<=26;i++)
	{
		root_p = insert_p(root_p, i, i, 1);
	}
	for(i=17;i<=33;i++)
	{
		root_p = insert_p(root_p, i, 0, 1);
	}
	for(i=34;i<=36;i++)
	{
		root_p = insert_p(root_p, i, i, 2);
	}
	
	
	for(i=37;i<=45;i++)
	{
		root_p = insert_p(root_p, i, i, 1);
	}
	for(i=46;i<=50;i++)
	{
		root_p = insert_p(root_p, i, 0, 2);
	}
	for(i=51;i<=64;i++)
	{
		root_p = insert_p(root_p, i, i, 2);
	}
	for(i=65;i<=72;i++)
	{
		root_p = insert_p(root_p, i, i, 1);
	}
	
	
	for(i=73;i<=81;i++)
	{
		root_p = insert_p(root_p, i, i, 1);
	}
	for(i=82;i<=94;i++)
	{
		root_p = insert_p(root_p, i, i, 2);
	}
	for(i=95;i<=100;i++)
	{
		root_p = insert_p(root_p, i, 0, 2);
	}
	for(i=101;i<=108;i++)
	{
		root_p = insert_p(root_p, i, i, 2);
	}
	
	//-->		end of arking details			<--//
	
	int choose ;
	int id,vtype;
	int sno;
	int j=0;
	
	while(1)
	{
		printf("\n\n1 for Add vehicle\n2 for park vehicle\n3 for remove vehicle\n4 for display visitors\n5 for parking status\n6 for Exit\nEnter your choice : ");
		scanf("%d",&choose);
	
	
		switch(choose)
		{
			case 1:
				printf("Enter Flat_id : ");
				scanf("%d",&id);
				printf("enter 1 for two wheeler and 2 for four wheeler : ");
				scanf("%d",&vtype);								
				add_map_vehicle(root_f,id,vtype);
				add_map_v_p(root_p,id);		
				break;
				
			case 2:
				printf("\nEnter your spot no. to park vehicle : ");
				scanf("%d",&sno);
				park_vehicle(root_p, sno);
				break;
				
			case 3:
				printf("Enter your id : ");
				scanf("%d",&id);
				Remove_Vehicle_node(root_f, id);
				Remove_v_p(root_p, id);
				break;
				
			case 4: 
				display_visitors(root_p);
				break;
				
			case 5:
				parking_status(root_p);
				break;
			
			case 6:
				exit(0);
				
			default :
				printf("Wrong Option");
				break; 	
		}
		printf("\n");
	}
	return 0;
}
