/* Program description [change this] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

struct itemType{
    char category[30];
    char brand[30];
    char product[50];
    float price;
    int barcode;
};
struct itemType item[100];

struct transactionType{
    char td[11]; // yyyy-mm-dd
    char tt[9]; // hh:mm:ss
    int id;
    float price;
    int qty;
    int barcode;
};
struct transactionType transaction[100];

FILE *fp;

int i, j;// for sorting or looping

int inv_cnt=0;
int trn_cnt=0;

int read_inventory();
void print_inventory();
void write_inventory();

int read_transaction();
void print_transaction();
void write_transaction();

void fgetString();
void getString();

void print_item();

void uploaditem();

int checkitem();

char GenDate();
char GenTime();

int StudID;

int itemcode;
int buy_item;
int buy_value;


int ed_item=0;

int del_item=0;


int up_item;
int up_value;

int c_result;

int input;//for Yes/No decision(0=Yes, 1=No)
int found=0;//0=not found, 1=found
char backmenu;

char key[99];
int search=0;
char catStr[99], branStr[99], prodStr[99], codeStr[99];

char pw[99];

int sold_no[99]= {0};

main(){
int tempStr;
	do{
		printf("Main Menu\n");
		printf("1. Self Help Sale\n");
		printf("2. Search an item\n");
		printf("3. Admin\n");
		printf("4. Best-selling Item\n");
		printf("9. Quit\n");
		printf("Select a function: ");
		scanf("%d", &tempStr);
        if(tempStr==1) {// sale sub program
            inv_cnt=read_inventory();
            trn_cnt=read_transaction();
            found=0;
            do {
                printf("StudentID: ");
                scanf("%d", &StudID);
                while(found==0) {
                    printf("Barcode: ");
                    scanf("%d", &itemcode);
                    for(i=1; i<=inv_cnt; i++) {
                        if(itemcode==item[i].barcode) {
                            buy_item=i;
                            found=1;
                        }
                    }
                    if(found==0){
                        printf("No this item. Try again?(Press 9 to go back to main menu / Press any key to continue) ");
                        fflush(stdin);
                        scanf("%c", &backmenu);
                        if(backmenu=='9') {
                            return main();
                        }
                    }
                }
                printf("How many numbers of this item do you want to buy? ");
                scanf("%d", &buy_value);
                printf("\nPlease confirm the information of the item and your ID\n\n");
                printf("StudentID: %d\n", StudID);
                print_item(buy_item);
                printf("Number of items: %d\n\n", buy_value);
                printf("Is the information correct? (Yes:0, No:1) ");
                scanf("%d", &input);
                while(input!=0 & input!=1){
                    printf("Please write 0 for 'Yes' or 1 for 'No'\n");
                    printf("Is the information correct? (Yes:0, No:1) ");
                    scanf("%d", &input);
                }
                if(input==1) {
                    found=0;
                    printf("Try again?(Press 9 to go back to main menu / Press any key to continue) ");
                    fflush(stdin);
                    scanf("%c", &backmenu);
                    if(backmenu=='9') {
                        return main();
                    }
                }
            }while (input==1);
            if(input==0){
                trn_cnt++;
                GenDate();
                GenTime();
                transaction[trn_cnt].id=StudID;
                transaction[trn_cnt].qty=0-buy_value;//make the number become negative
                transaction[trn_cnt].price=item[buy_item].price;
                transaction[trn_cnt].barcode=item[buy_item].barcode;
                write_transaction();
            }

        }else if(tempStr==2) {// search sub program
            do {
                inv_cnt=read_inventory();
                search=0;
                printf("Search item or type the keywords: ");
                getString(key);
                for(j=0; j<=strlen(key); j++) {
                    key[j]=tolower(key[j]);
                }
                for(i=1; i<=inv_cnt; i++) {
                    for(j=0; j<=strlen(key); j++) {
                        catStr[j]=tolower(item[i].category[j]);
                        branStr[j]=tolower(item[i].brand[j]);
                        prodStr[j]=tolower(item[i].product[j]);
                    }
                    if(strncmp(key, catStr, strlen(key))==0) {
                        search=i;
                    }
                    if(strncmp(key, branStr, strlen(key))==0) {
                        search=i;
                    }
                    if(strncmp(key, prodStr, strlen(key))==0) {
                        search=i;
                    }
                    sprintf(codeStr, "%d", item[i].barcode);
                    if(strncmp(key, codeStr, strlen(key))==0){
                        search=i;
                    }
                    if(search==i) {
                        print_item(search);
                    }
                }
                if(search==0) {
                    printf("No this item. Try again?(Press 9 to go back to main menu / Press any key to continue) ");
                    fflush(stdin);
                    scanf("%c", &backmenu);
                    if(backmenu=='9') {
                        return main();
                    }
                } else {
                    printf("Continue?(Press 9 to go back to main menu / Press any key to continue) ");
                    fflush(stdin);
                    scanf("%c", &backmenu);
                    if(backmenu=='9') {
                        return main();
                    }
                }
            }while(backmenu!='9');

		}else if(tempStr==3) {
		    printf("\nLog on using your ID and password\n");
		    printf("Student ID: ");
		    scanf("%d", &StudID);
		    printf("Password: ");
		    getString(pw);
		    if (strcmp(pw, "123456")==0) {
                start_admin();
		    } else {
                printf("Invalid password. The system will go back to the main menu now.\n");
                return main();
		    }

        }else if(tempStr==4) {
            inv_cnt=read_inventory();
            trn_cnt=read_transaction();
            for(i=1; i<=inv_cnt; i++) {
                for(j=1; j<=trn_cnt; j++) {
                    if(transaction[j].price>0 && item[i].barcode==transaction[j].barcode) {
                        sold_no[i]=sold_no[i]+abs(transaction[j].qty);
                    }
                }
            }
            for(i=1; i<=inv_cnt; i++) {
                for(j=2; j<=inv_cnt; j++) {
                    if(sold_no[j]>sold_no[j-1]) {
                        item[0]=item[j];
                        item[j]=item[j-1];
                        item[j-1]=item[0];
                        sold_no[0]=sold_no[j];
                        sold_no[j]=sold_no[j-1];
                        sold_no[j-1]=sold_no[0];
                    }
                }
            }
            printf("Top 3 items:\n\n");
            for(i=1; i<=3; i++) {
                print_item(i);
                printf("\n");
            }
		}
	}while(tempStr!=9);
}

void start_admin(){
int tempStr;
	inv_cnt=read_inventory();
	trn_cnt=read_transaction();
	start:
	do{
		printf("Sub Menu\n");
		printf("1. Inventory LIST\n");
		printf("2. Inventory LIST(category)\n");
		printf("3. Inventory LIST(barcode)\n");
		printf("4. Inventory NEW ITEM\n");
		printf("5. Edit item\n");
        printf("6. Delete item\n");
		printf("7. Update number of item\n");
		printf("8. Transaction LIST\n");
		printf("9. Quit\n");
		printf("Select a function: ");
		scanf("%d", &tempStr);
		if(tempStr==1) {
			print_inventory();// print inventory sub program

        } else if(tempStr==2) {
            for(i=1; i<=inv_cnt-1; i++){
                for(j=inv_cnt-1; j>=i; j--){
                    if(strcmp(item[j].category, item[j+1].category)>0){
                        item[0]=item[j];
                        item[j]=item[j+1];
                        item[j+1]=item[0];
                    }
                }
            }print_inventory();

        } else if(tempStr==3) {
            for(i=1; i<=inv_cnt-1; i++){
                for(j=inv_cnt-1; j>=i; j--){
                    if(item[j].barcode>item[j+1].barcode){
                        item[0]=item[j];
                        item[j]=item[j+1];
                        item[j+1]=item[0];
                    }
                }
            }print_inventory();

        } else if(tempStr==4) {
            do{
                uploaditem(1);
                c_result=checkitem(1);
                if (c_result==1) {
                    printf("Try again?(Press 9 to go back to sub menu / Press any key to continue) ");
                    fflush(stdin);
                    scanf("%c", &backmenu);
                    if(backmenu=='9') {
                        return start_admin();
                    }
                }
            }while(c_result==1);
            if(c_result==0){
                inv_cnt++;
                write_inventory();
            }

        }else if(tempStr==5) {
            printf("Which item do you want to edit? ");
            scanf("%d", &ed_item);
            if(ed_item<=inv_cnt){
                print_item(ed_item);
                printf("Are you sure you want to edit this item? (Yes:0, No:1)");
                scanf("%d", &input);
                while(input!=0 & input!=1){
                    printf("Please write 0 for 'Yes' or 1 for 'No'\n");
                    printf("Are you sure you want to edit this item? (Yes:0, No:1)");
                    scanf("%d", &input);
                }
                if(input==0){
                    do{
                        uploaditem(0);
                        c_result=checkitem(0);
                        if (c_result==1) {
                            printf("Try again?(Press 9 to go back to sub menu / Press any key to continue) ");
                            fflush(stdin);
                            scanf("%c", &backmenu);
                            if(backmenu=='9') {
                                return start_admin();
                            }
                        }
                    }while(c_result==1);
                    if(c_result==0){
                        write_inventory();
                    }
                } else if(input==1) {
                    printf("If you want all information about the items, you can refer to Inventory List(admin 1).\n\n");
                }
            } else {
                printf("No this item, please refer to Inventory List(admin 1).\n\n");
            }

        } else if(tempStr==6) {
            printf("Which item do you want to delete? ");
            scanf("%d", &del_item);
            if(del_item<=inv_cnt){
                print_item(del_item);
                printf("Are you sure you want to delete this item? (Yes:0, No:1)");
                scanf("%d", &input);
                while(input!=0 & input!=1){
                    printf("Please write 0 for 'Yes' or 1 for 'No'\n");
                    printf("Are you sure you want to delete this item? (Yes:0, No:1)");
                    scanf("%d", &input);
                }
                if(input==0){
                    for(i=del_item; i<=inv_cnt; i++){
                        item[i]=item[i+1];
                    }
                    inv_cnt--;
                    write_inventory();
                } else if(input==1){
                    printf("If you want all information about the items, you can refer to Inventory List(admin 1).\n\n");
                }
            } else {
                printf("No this item, please refer to Inventory List(admin 1).\n\n");
            }

        } else if(tempStr==7) {
            printf("Which item do you want to update? ");
            scanf("%d", &up_item);
            if(up_item<=inv_cnt) {
                print_item(up_item);
                printf("Are you sure you want to update this item? (Yes:0, No:1) ");
                scanf("%d", &input);
                while(input!=0 & input!=1){
                    printf("Please write 0 for 'Yes' or 1 for 'No'\n");
                    printf("Are you sure you want to update this item? (Yes:0, No:1) ");
                    scanf("%d", &input);
                }
                if(input==0){
                    trn_cnt++;
                    printf("Write down the number of item you want to update(Use negative number for reduction of the item) ");
                    scanf("%d", &up_value);
                    GenDate();
                    GenTime();
                    transaction[trn_cnt].id=StudID;
                    transaction[trn_cnt].qty=up_value;
                    transaction[trn_cnt].price=0.0;
                    transaction[trn_cnt].barcode=item[up_item].barcode;
                    write_transaction();
                } else if(input==1){
                    printf("If you want all information about the items, you can refer to Inventory List(admin 1).\n\n");
                }
            } else {
                printf("No this item, please refer to Inventory List(admin 1).\n\n");
            }

        } else if(tempStr==8) {
            print_transaction();// print transaction sub program
        }
	}while(tempStr!=9);
}

int read_inventory(){
char temp[99];
int count=0;
    fp = fopen("inventory test.txt", "r");
    fgets(temp, 99, fp);
    while(!feof(fp)) {
		count++;
    	fgetString(temp);
    	strcpy(item[count].category, temp);
    	fgetString(temp);
    	strcpy(item[count].brand, temp);
    	fgetString(temp);
    	strcpy(item[count].product, temp);
    	fgetString(temp);
    	item[count].price=atof(temp);
		fgetString(temp);
		item[count].barcode=atoi(temp);
    }
    fclose(fp);
    return count-1;

}

void print_inventory() {
int i=1;
    printf("STMC SU POS system inventory\n");
    while(i<=inv_cnt) {
        printf("%2d. %s\n", i, item[i].category);
        printf("%s\n", item[i].brand);
        printf("%s\n", item[i].product);
        printf("$%.1f\n", item[i].price);
        printf("%d\n\n", item[i].barcode);
        i++;
    }
}

void write_inventory() {
int i=1;
    fp = fopen("inventory test.txt", "w");
    fprintf(fp, "#category, brand, product, price, cost, barcode\n");
    while(i<=inv_cnt) {
        fprintf(fp, "%s\n", item[i].category);
        fprintf(fp, "%s\n", item[i].brand);
        fprintf(fp, "%s\n", item[i].product);
        fprintf(fp, "%.1f\n", item[i].price);
        fprintf(fp, "%d\n", item[i].barcode);
        i++;
    }
    fclose(fp);
    printf("Information is saved. Thank you\n\n");
}

int read_transaction() {
char temp[99];
int count=0;
    fp = fopen("transaction test.txt", "r");
    fgets(temp, 99, fp);
    while(!feof(fp)) {
        count++;
        fgetString(temp);
        strcpy(transaction[count].td, temp);
        fgetString(temp);
        strcpy(transaction[count].tt, temp);
        fgetString(temp);
        transaction[count].id=atoi(temp);
        fgetString(temp);
        transaction[count].price=atof(temp);
        fgetString(temp);
        transaction[count].qty=atoi(temp);
        fgetString(temp);
        transaction[count].barcode=atoi(temp);
    }
    fclose(fp);
    return count-1;

}

void print_transaction() {
int i=1;
    printf("STMC SU POS system transaction\nInstruction: If price=$0, it means the update of the number of that item\n");
    while(i<=trn_cnt) {
        printf("%2d. %s\t ", i, transaction[i].td);
        printf(" %s\t ", transaction[i].tt);
        printf(" %d\t ", transaction[i].id);
        printf(" $%.1f\t", transaction[i].price);
        printf(" %d\t", transaction[i].qty);
        printf(" %d\t\n\n", transaction[i].barcode);
        i++;
    }
}

void write_transaction() {
int i=1;
    fp = fopen("transaction test.txt", "w");
    fprintf(fp, "#date,time,id,price,qty,barcode\n");
    while(i<=trn_cnt) {
        fprintf(fp, "%s\n", transaction[i].td);
        fprintf(fp, "%s\n", transaction[i].tt);
        fprintf(fp, "%d\n", transaction[i].id);
        fprintf(fp, "%.1f\n", transaction[i].price);
        fprintf(fp, "%d\n", transaction[i].qty);
        fprintf(fp, "%d\n", transaction[i].barcode);
        i++;
    }
    fclose(fp);
}

void fgetString(char string[99]) {
    fgets(string, 99, fp);
    string[strlen(string)-1]='\0';
}

void getString(char string[99]) {
    fflush(stdin);
    gets(string);
    string[strlen(string)]='\0';
}

void print_item(int i) {
    printf("%2d. %s\n", i, item[i].category);
    printf("%s\n", item[i].brand);
    printf("%s\n", item[i].product);
    printf("$%.1f\n", item[i].price);
    printf("%d\n", item[i].barcode);
}

void uploaditem(int option){ // option=0: edit, =1: new
char tempStr[99];
int upl_no;
    if(option==1){
        upl_no=inv_cnt+1;
    } else if(option==0){
        upl_no=ed_item;
    }
    printf("Category:");
    getString(item[upl_no].category);
    printf("Brand:");
    getString(item[upl_no].brand);
    printf("Product:");
    getString(item[upl_no].product);
    printf("Price:");
    getString(tempStr);
    item[upl_no].price=atof(tempStr);
    printf("Barcode:");
    getString(tempStr);
    item[upl_no].barcode=atoi(tempStr);
}


int checkitem(int option) { // option=0: edit, =1: new
int checkno;
    if(option==1) {
        checkno=inv_cnt+1;
    } else if(option==0) {
        checkno=ed_item;
    }
    printf("\nPlease check the result\n\n");
    print_item(checkno);
    printf("\nIs the information correct? (Yes:0, No:1)");
    scanf("%d", &input);
    while(input!=0 & input!=1){
        printf("Please write 0 for 'Yes' or 1 for 'No'\n");
        printf("Is the information correct? (Yes:0, No:1)");
        scanf("%d", &input);
        }
        return input;
}

char GenDate() {
int y, m, d;
char year[99], month[99], day[99];
time_t timep;
struct tm *p;
    time(&timep);
    p = localtime(&timep);
    y=1900+p->tm_year;
    m=1+ p->tm_mon;
    d=p->tm_mday;
    sprintf(year, "%d", y);
    sprintf(month, "%d", m);
    sprintf(day, "%d", d);
    strcat(year, "-");
    strcat(month, "-");
    strcpy(transaction[trn_cnt].td, strcat(strcat(year, month), day));
}

char GenTime() {
int h, m, s;
char hour[99], min[99], sec[99];
time_t timep;
struct tm *p;
    time(&timep);
    p = localtime(&timep);
    h=p->tm_hour;
    m=1+ p->tm_min;
    s=p->tm_sec;
    sprintf(hour, "%d", h);
    sprintf(min, "%d", m);
    sprintf(sec, "%d", s);
    strcat(hour, ":");
    strcat(min, ":");
    strcpy(transaction[trn_cnt].tt, strcat(strcat(hour, min), sec));
}


