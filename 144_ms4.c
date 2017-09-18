#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


// Define statements:
#define LINEAR 1
#define FORM 0
#define STOCK 1
#define CHECKOUT 0
#define MAX_ITEM_NO 500
#define MAX_QTY 999
#define SKU_MAX 999
#define SKU_MIN 100
#define DATAFILE "items.txt"

// Constant TAX value:
const double TAX = 0.13;


// Item structure:
struct Item {
   double price;
   int sku;
   int isTaxed;
   int quantity;
   int minQuantity;
   char name[21];
};


// Function Prototypes Milestone 1:
void welcome(void);
void printTitle(void);
void printFooter(double gTotal);
void pause(void);
void flushKeyboard(void); // based off of fardad's lecture
int getInt(void);
double getDouble(void);
int getIntLimited(int lowerLimit, int upperLimit);
double getDoubleLimited(double lowerLimit, double upperLimit);
int yes(void);
void search(const struct Item item[], int NoOfRecs);
void updateItem(struct Item* itemptr);
void GroceryInventorySystem();






// Function Prototypes Milesone 2:
double totalAfterTax(struct Item item);
int isLowQuantity(struct Item item);
struct Item itemEntry(int sku);
void displayItem(struct Item item, int linear);
void listItems(const struct Item item[], int NoOfItems);
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index);
void addItem(struct Item item[], int *NoOfRecs, int sku);
void addOrUpdateItem(struct Item item[], int* NoOfRecs);
void adjustQuantity(struct Item item[], int NoOfRecs, int stock);
void saveItem(struct Item item, FILE* dataFile);
int loadItem(struct Item* item, FILE* dataFile);
int saveItems(const struct Item item[], char fileName[], int NoOfRecs);
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr);
void searchName(struct Item item[]);
void delItem(struct Item item[], FILE *fp, int *NoOfRec);




int main() {
   GroceryInventorySystem();
   return 0;
}


// Function Implementations Milestone 1:

// code your functions here:

void welcome(void) {
   printf("---=== Grocery Inventory System ===---\n\n");
}
void printTitle(void) {
   printf("Row |SKU| Name               | Price  |Taxed| Qty | Min |   Total    |Atn\n");
   printf("----+---+--------------------+--------+-----+-----+-----+------------|---\n");
}
void printFooter(double grandTotal) {
   printf("--------------------------------------------------------+----------------\n");
   if (grandTotal > 0) {
      printf("                                           Grand Total: |%12.2lf\n", grandTotal);
   }
}
void flushKeyboard(void) {
   char garbage;
   do {
      scanf("%c", &garbage);
   } while (garbage != '\n');
} // based off of fardad's lecture
void pause(void) {
   printf("Press <ENTER> to continue...");
   flushKeyboard();
}
int getInt(void) {
   int res;
   char NL = 'x';
   do {
      scanf("%d%c", &res, &NL);
      if (NL != '\n')
      {
         flushKeyboard();
         printf("Invalid integer, please try again: ");
      }
   } while (NL != '\n');
   return res;
}
int getIntLimited(int lowerLimit, int upperLimit) {
   int lim;
   int j = 1;
   do {
      lim = getInt();
      if (lim < lowerLimit || lim > upperLimit) {
         printf("Invalid value, %d < value < %d: ", lowerLimit, upperLimit);
      }
      if (lim >= lowerLimit && lim <= upperLimit) {
         j = 0;
      }
   } while (j == 1);
   return lim;
}
double getDouble(void) {
   double res;
   char NL = 'x';
   while (NL != '\n') {
      scanf("%lf%c", &res, &NL);
      if (NL != '\n') {
         flushKeyboard();
         printf("Invalid number, please try again: ");
      }
   }
   return res;
}
double getDoubleLimited(double lowerLimit, double upperLimit) {
   double lim;
   int j = 1;
   do {
      lim = getDouble();
      if (lim < lowerLimit || lim > upperLimit) {
         printf("Invalid value, %.6lf< value < %.6lf: ", lowerLimit, upperLimit);
      }
      else {
         j = 0;
      }
   } while (j == 1);

   return lim;

}
int yes(void) {
   char yes;
   //char NL;
   int res;
   int j = 1;

   do {
      scanf("%c", &yes);
      switch (yes) {
      default:
         printf("Only (Y)es or (N)o are acceptable: ");
         flushKeyboard();
         break;
      case 'y':
      case 'Y':
         res = 1;
         j = 0; //to get out of loop
         break;
      case 'n':
      case'N':
         res = 0;
         j = 0; // to get out of loop
         break;
      }
   } while (j == 1);
   return (res);
} // yes returns 1 no returns 0  // does not flush after y/n
void menu(void) {
   printf("1- List all items\n");
   printf("2- Search by SKU\n");
   printf("3- Checkout an item\n");
   printf("4- Stock an item\n");
   printf("5- Add new item or update item\n");
   printf("6- delete item\n");
   printf("7- Search by name\n");
   printf("0- Exit program\n");
   printf("> ");
}

void GroceryInventorySystem(void) {
   int select;
   int exit = 0;
   struct Item arrItems[MAX_ITEM_NO];
   int irr; //number of records read.
   FILE *gis = fopen(DATAFILE, "r+");
   welcome();
   if (gis == NULL) {
      printf("Could not read from %s.\n", DATAFILE);
   }
   else {
      loadItems(arrItems, DATAFILE, &irr);
      do {
         menu();
         select = getIntLimited(0, 7);

         switch (select) {
         case 1:
            //printf("List Items under construction!\n");
            listItems(arrItems, irr);
            pause();
            break;
         case 2:
            //printf("Search Items under construction!\n");
            search(arrItems, irr);
            pause();
            break;
         case 3:
            //printf("Checkout Item under construction!\n");
            adjustQuantity(arrItems, irr, CHECKOUT);
            saveItems(arrItems, DATAFILE, irr);
            pause();
            break;
         case 4:
            //printf("Stock Item under construction!\n");
            adjustQuantity(arrItems, irr, STOCK);
            saveItems(arrItems, DATAFILE, irr);
            pause();
            break;
         case 5:
            //printf("Add/Update Item under construction!\n");
            addOrUpdateItem(arrItems, &irr);
            saveItems(arrItems, DATAFILE, irr);
            pause();
            break;
         case 6:
            //printf("Delete Item under construction!\n");
            delItem(arrItems, gis, &irr);
            rename("temp.txt", DATAFILE); // saves deleted item to replace new file
            pause();
            break;
         case 7:
            //printf("Search by name under construction!\n");
            searchName(arrItems);
            pause();
            break;
         case 0:
            printf("Exit the program? (Y)es/(N)o: ");
            exit = yes();
            flushKeyboard();
            //if (exit == 0);
            //menu();
            break;
         default:
            flushKeyboard();
            break;

         }
      } while (exit == 0);
   }
}

// Function Implementations Milestone 2:
double totalAfterTax(struct Item item) {
   double res;
   if (item.isTaxed == 1) {
      res = ((item.price*(1 + TAX)) * item.quantity);
   }
   else {
      res = (item.price * item.quantity);
   }
   return res;
}
int isLowQuantity(struct Item item) {
   int res = 0;
   if (item.quantity < item.minQuantity) {
      res = 1;
   }
   return res;
}
struct Item itemEntry(int sku) {
   struct Item temp;
   temp.sku = sku;
   //putchar('\n');
   printf("        SKU: %d\n", temp.sku);
   printf("       Name: ");
   scanf("%20[^\n]", temp.name);
   flushKeyboard();
   printf("      Price: ");
   temp.price = getDoubleLimited(0.01, 1000.00);
   printf("   Quantity: ");
   temp.quantity = getIntLimited(1, MAX_QTY);
   printf("Minimum Qty: ");
   temp.minQuantity = getIntLimited(0, 100);
   printf("   Is Taxed: ");
   temp.isTaxed = yes();

   return (temp);
}//call flushKeyboard(); after
void displayItem(struct Item item, int linear) {

   char *ynp;
   double toAfTax = totalAfterTax(item);
   if (linear == LINEAR) {
      if (item.isTaxed == 1) {
         ynp = "Yes";
      }
      else if (item.isTaxed == 0) {
         ynp = "No";
      }
      printf("|%3d|%-20s|%8.2lf|  %3s| %3d | %3d |%12.2lf|", item.sku, item.name, item.price, ynp, item.quantity, item.minQuantity, toAfTax);
      if (item.quantity <= item.minQuantity) {
         printf("***");
      }
      printf("\n");
   }

   if (linear == FORM) {
      if (item.isTaxed == 1) {
         ynp = "Yes";
      }
      else if (item.isTaxed == 0) {
         ynp = "No";
      }
      printf("        SKU: %d\n", item.sku);
      printf("       Name: %s\n", item.name);
      printf("      Price: %.2lf\n", item.price);
      printf("   Quantity: %d\n", item.quantity);
      printf("Minimum Qty: %d\n", item.minQuantity);
      printf("   Is Taxed: %s\n", ynp);
      if (item.quantity <= item.minQuantity) {
         printf("WARNING: Quantity low, please order ASAP!!!\n");
      }
   }
   //putchar('\n');
}
void listItems(const struct Item item[], int NoOfItems) {
   //This function receives a constant array of Items and their number and prints the items in list with the grand total price at the end.
   int cnt = 0;
   double grandTotal = 0;
   double priceTaxed;
   printTitle();
   for (cnt = 0; cnt < NoOfItems; cnt++) {
      printf("%-4d", cnt + 1);
      displayItem(item[cnt], LINEAR);
      priceTaxed = totalAfterTax(item[cnt]);
      grandTotal += priceTaxed;
   }
   printFooter(grandTotal);
}
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index) {
   int res = 0;
   int lc = 0; //locateitem counter
   for (lc = 0; lc < NoOfRecs; lc++) {
      if (item[lc].sku == sku) {
         *index = lc;
         res = 1;
      }
   }
   return (res);
}
void search(const struct Item item[], int NoOfRecs) {
   int skuTemp;
   int sindex = 0;
   int Found;
   printf("Please enter the SKU: ");
   skuTemp = getIntLimited(SKU_MIN, SKU_MAX);
   Found = locateItem(item, NoOfRecs, skuTemp, &sindex);
   if (Found == 0) {
      printf("Item not found!\n");
   }
   else if (Found == 1) {
      displayItem(item[sindex], FORM);
   }
}
void updateItem(struct Item* itemptr) {
   struct Item tempup;
   int confirm;
   printf("Enter new data:\n");
   //flushKeyboard();
   tempup = itemEntry(itemptr->sku);
   flushKeyboard();
   printf("Overwrite old data? (Y)es/(N)o: ");
   confirm = yes();
   flushKeyboard();
   if (confirm == 1) {
      *itemptr = tempup;
      printf("--== Updated! ==--\n");
   }
   else if (confirm == 0) {
      printf("--== Aborted! ==--\n");
   }
}
void addItem(struct Item item[], int *NoOfRecs, int sku) {
   struct Item tempitem;
   int addconfirm;
   if (*NoOfRecs == (MAX_ITEM_NO)) {
      printf("Can not add new item; Storage Full!\n");
   }
   else {
      tempitem = itemEntry(sku);
      flushKeyboard();
      printf("Add Item? (Y)es/(N)o: ");
      addconfirm = yes();
      flushKeyboard();
      if (addconfirm == 1) {
         item[*NoOfRecs] = tempitem;
         (*NoOfRecs)++;
         printf("--== Added! ==--\n");
      }
      else if (addconfirm == 0) {
         printf("--== Aborted! ==--\n");
      }
   }
} //check item[] if something goes wrong
void addOrUpdateItem(struct Item item[], int* NoOfRecs) {
   int autempsku; //addupdate
   int aufound; //addupdatefound
   int auindex;
   int auyn;
   printf("Please enter the SKU: ");
   autempsku = getIntLimited(SKU_MIN, SKU_MAX);
   aufound = locateItem(item, *NoOfRecs, autempsku, &auindex);
   if (aufound == 1) {
      displayItem(item[auindex], FORM);
      printf("Item already exists, Update? (Y)es/(N)o: ");
      auyn = yes();
      flushKeyboard();
      if (auyn == 1) {
         updateItem(&item[auindex]);
      }
      if (auyn == 0) {
         printf("--== Aborted! ==--\n");
      }
   }
   if (aufound == 0) {
      addItem(item, NoOfRecs, autempsku);
   }
}
void adjustQuantity(struct Item item[], int NoOfRecs, int stock) {
   int aqtempSKU;
   int aqlocate;
   int aqindex;
   int aqtemp; // quantity to adjust
   char *stockcheck;
   if (stock == 1) {
      stockcheck = "stock";
   }
   else {
      stockcheck = "checkout";
   }

   printf("Please enter the SKU: ");
   aqtempSKU = getIntLimited(SKU_MIN, SKU_MAX);
   aqlocate = locateItem(item, NoOfRecs, aqtempSKU, &aqindex);
   if (aqlocate == 0) {
      printf("SKU not found in storage!\n");
   }
   else {
      if (stock == 1) {
         displayItem(item[aqindex], FORM);
         printf("Please enter the quantity to %s; Maximum of %d or 0 to abort: ", stockcheck, (MAX_QTY - item[aqindex].quantity));
         aqtemp = getIntLimited(0, (MAX_QTY - item[aqindex].quantity));
         if (aqtemp == 0) {
            printf("--== Aborted! ==--\n");
         }
         if (aqtemp > 0) {
            printf("--== Stocked! ==--\n");
            item[aqindex].quantity += aqtemp;
            if (item[aqindex].quantity <= item[aqindex].minQuantity) {
               printf("Quantity is low, please reorder ASAP!!!\n");
            }
         }
      }
      else {
         displayItem(item[aqindex], FORM);
         printf("Please enter the quantity to %s; Maximum of %d or 0 to abort: ", stockcheck, (item[aqindex].quantity));
         aqtemp = getIntLimited(0, item[aqindex].quantity);
         if (aqtemp == 0) {
            printf("--== Aborted! ==--\n");
         }
         if (aqtemp > 0) {
            printf("--== Checked out! ==--\n");
            item[aqindex].quantity -= aqtemp;
            if (item[aqindex].quantity <= item[aqindex].minQuantity) {
               printf("Quantity is low, please reorder ASAP!!!\n");
            }
         }
      }
   }
}
void saveItem(struct Item item, FILE* dataFile) {
   if (fprintf(dataFile, "%d,%d,%d,%.2lf,%d,%s\n", item.sku, item.quantity, item.minQuantity, item.price, item.isTaxed, item.name) == 6) {
      ;
   }
}
int loadItem(struct Item* item, FILE* dataFile) {
   int li = 0;
   int scRead = 0;
   scRead = fscanf(dataFile, "%d,%d,%d,%lf,%d,%[^\n]", &item->sku, &item->quantity, &item->minQuantity, &item->price, &item->isTaxed, item->name);
   if (scRead == 6) {
      li = 1;
   }
   return (li);
}
int saveItems(const struct Item item[], char fileName[], int NoOfRecs) {
   int sitems = 0;
   int indexsi;
   FILE *fp;
   fp = fopen(fileName, "w");
   if (fp != NULL) {
      for (indexsi = 0; indexsi < NoOfRecs; indexsi++) {
         saveItem(item[indexsi], fp);
      }
      sitems = 1;
   }
   else {
      printf("could not update data file %s\n", DATAFILE);
   }
   fclose(fp);
   return (sitems);
}
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr) {
   int tf = 0;
   //int check = 1; //<-- if i set loadItem's return value to this and use this for condition in loop it doesn't work; get runtime error
   //however, if use function inside the condition it works.
   int indexli = 0;
   FILE *fp = fopen(fileName, "r");
   if (fp != NULL) {
      for (; loadItem(&item[indexli], fp) == 1; indexli++) {
         ;
      }
      *NoOfRecsPtr = indexli;
      tf = 1;
   }
   fclose(fp);
   return (tf);
}
void delItem(struct Item item[], FILE *fp, int *NoOfRec) {
   FILE *ft = fopen("temp.txt", "w");
   int del; // sku to delete
   int delcnt = 0; //counter
   int delindex = 0; // index of the item to delete
   struct Item temp[MAX_ITEM_NO]; // temp struct to store items
   printf("Please enter a SKU: ");
   del = getIntLimited(100, 999);
   if (locateItem(item, MAX_ITEM_NO, del, &delindex) == 1) { // if i find the sku to delete then do the magic
      printf("Deleting Item: %d\n", del);
      while (fscanf(fp, "%d,%d,%d,%lf,%d,%[^\n]", &temp[delcnt].sku, &temp[delcnt].quantity, &temp[delcnt].minQuantity, &temp[delcnt].price, &temp[delcnt].isTaxed, temp[delcnt].name) == 6) {
         //printf("%d,%d,%d,%lf,%d,%s\n", temp[delcnt].sku, temp[delcnt].quantity, temp[delcnt].minQuantity, temp[delcnt].price, temp[delcnt].isTaxed, temp[delcnt].name);
         if (delcnt != delindex) {
            fprintf(ft, "%d,%d,%d,%.2lf,%d,%s\n", temp[delcnt].sku, temp[delcnt].quantity, temp[delcnt].minQuantity, temp[delcnt].price, temp[delcnt].isTaxed, temp[delcnt].name);
         }
         delcnt++;
      }
      //printf("%d\n", delindex);
      fclose(fp); // need to close fp to be able to rename temp.txt to items.txt
      fclose(ft); // need to close fp to be able to rename temp.txt to items.txt
      remove(DATAFILE); // need to remove original file to rename
      rename("temp.txt", DATAFILE); // rename temp with deleted to original
      fp = fopen(DATAFILE, "r+");
                                    //printf("%d\n", delcnt - 1);
      *NoOfRec = delcnt - 1; // set index to -1 since record got deleted
   }
   else {
      printf("Invalid SKU\n");
   }
}




void searchName(struct Item item[]) {
   char st[21];
   int snc = 0; //counter for array
   int found = 0;

   printf("Please enter the Name: ");
   scanf("%20[^\n]", st);
   flushKeyboard();
   for (; snc < MAX_ITEM_NO; snc++) {
      if (strcmp(st, item[snc].name) == 0) {
         printf("Item found!!!\n\n");
         displayItem(item[snc], FORM);
         found = 1;
      }
      if (snc == MAX_ITEM_NO - 1 && found == 0) {
         printf("Item not found!!!\n");
      }
   }
   putchar('\n');
}