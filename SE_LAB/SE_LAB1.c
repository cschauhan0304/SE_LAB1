#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Item {
    char name[50];
    float price;
    int quantity;
};

void displayMenuShopkeeper() {
    printf("\n1. Add Item\n");
    printf("2. Update Quantity\n");
    printf("3. Generate Report\n");
    printf("4. Exit\n");
}

void displayMenuUser() {
    printf("\n1.DisplayCart\n");
    printf("2. ToGenerateBill\n");
    printf("3. addToCart\n");
    printf("4. Display Inventory from which we can add in cart\n");
    printf("5. Exit\n");
}

void addItem(struct Item inventory[], int *count) {
    printf("Enter item name: ");
    scanf("%s", inventory[*count].name);

    printf("Enter item price: ");
    scanf("%f", &inventory[*count].price);

    printf("Enter initial quantity: ");
    scanf("%d", &inventory[*count].quantity);

    (*count)++;
}

void updateQuantity(struct Item inventory[], int count) {
    char itemName[50];
    int quantity;

    printf("Enter item name: ");
    scanf("%s", itemName);

    for (int i = 0; i < count; i++) {
        if (strcmp(inventory[i].name, itemName) == 0) {
            printf("Enter new quantity: ");
            scanf("%d", &quantity);
            inventory[i].quantity = quantity;
            printf("Quantity updated successfully.\n");
            return;
        }
    }

    printf("Item not found.\n");
}

void generateReport(struct Item inventory[], int count) {
    printf("\n----- Inventory Report -----\n");
    printf("%-20s %-10s %-10s\n", "Item Name", "Price", "Quantity");

    for (int i = 0; i < count; i++) {
        printf("%-20s %-10.2f %-10d\n", inventory[i].name, inventory[i].price, inventory[i].quantity);
    }

    printf("\n");
}

void displayInventory(struct Item inventory[], int count) {
    printf("\n----- Inventory -----\n");
    printf("%-20s %-10s %-10s\n", "Item Name", "Price", "Quantity");

    for (int i = 0; i < count; i++) {
        printf("%-20s %-10.2f %-10d\n", inventory[i].name, inventory[i].price, inventory[i].quantity);
    }

    printf("\n");
}

void addToCart(struct Item shopkeeperInventory[], int shopkeeperItemCount, struct Item userInventory[], int *userItemCount) {
    char itemName[50];
    int quantity;

    printf("Enter the name of the item to add to the cart: ");
    scanf("%s", itemName);

    for (int i = 0; i < shopkeeperItemCount; i++) {
        if (strcmp(shopkeeperInventory[i].name, itemName) == 0) {
            printf("Enter quantity to add to the cart: ");
            scanf("%d", &quantity);

            if (quantity > 0 && quantity <= shopkeeperInventory[i].quantity) {
                
                strcpy(userInventory[*userItemCount].name, shopkeeperInventory[i].name);
                userInventory[*userItemCount].price = shopkeeperInventory[i].price;
                userInventory[*userItemCount].quantity = quantity;
                shopkeeperInventory[i].quantity -= quantity;
                
                saveInventoryToFile(shopkeeperInventory, shopkeeperItemCount, "shopkeeper_inventory.txt");
                 printf("Item added to the cart successfully.\n");
                (*userItemCount)++;
            } else {
                printf("Invalid quantity or insufficient stock.\n");
            }

            return;
        }
    }

    printf("Item not found in the shopkeeper's inventory.\n");
}

void generateBill(struct Item userInventory[], int userItemCount) {
    printf("\n----- Bill -----\n");
    printf("%-20s %-10s %-10s %-10s\n", "Item Name", "Price", "Quantity", "Total Price");

    float totalPrice = 0.0;

    for (int i = 0; i < userItemCount; i++) {
        float itemTotalPrice = userInventory[i].price * userInventory[i].quantity;
        printf("%-20s %-10.2f %-10d %-10.2f\n", userInventory[i].name, userInventory[i].price, userInventory[i].quantity, itemTotalPrice);
        totalPrice += itemTotalPrice;
    }

    printf("\n");
    printf("Total Price: %-10.2f\n", totalPrice);
}


void saveInventoryToFile(struct Item inventory[], int count, const char *fileName) {
    FILE *file = fopen(fileName, "w+");

    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %.2f %d\n", inventory[i].name, inventory[i].price, inventory[i].quantity);
    }

    fclose(file);
}

int loadInventoryFromFile(struct Item inventory[], const char *fileName) {
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        return 0;
    }

    int count = 0;

    while (fscanf(file, "%s %f %d", inventory[count].name, &inventory[count].price, &inventory[count].quantity) == 3) {
        count++;
    }

    fclose(file);
    return count;
}

int main() {
    struct Item shopkeeperInventory[100];
    struct Item userInventory[100];
    int shopkeeperItemCount = 0;
    int userItemCount = 0;
    int roleChoice;

    printf("Choose your role:\n");
    printf("1. Shopkeeper\n");
    printf("2. User\n");
    printf("Enter your choice: ");
    scanf("%d", &roleChoice);

    switch (roleChoice) {
        case 1: // Shopkeeper
            shopkeeperItemCount = loadInventoryFromFile(shopkeeperInventory, "shopkeeper_inventory.txt");

            do {
                displayMenuShopkeeper();
                printf("Enter your choice: ");
                scanf("%d", &roleChoice);

                switch (roleChoice) {
                    case 1:
                        addItem(shopkeeperInventory, &shopkeeperItemCount);
                        break;
                    case 2:
                        updateQuantity(shopkeeperInventory, shopkeeperItemCount);
                        break;
                    case 3:
                        generateReport(shopkeeperInventory, shopkeeperItemCount);
                        break;
                    case 4:
                        saveInventoryToFile(shopkeeperInventory, shopkeeperItemCount, "shopkeeper_inventory.txt");
                        printf("Exiting program.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            } while (roleChoice != 4);

            break;

        case 2: // User
            userItemCount = loadInventoryFromFile(userInventory, "user_inventory.txt");
            shopkeeperItemCount = loadInventoryFromFile(shopkeeperInventory, "shopkeeper_inventory.txt");
            do {
                displayMenuUser();
                printf("Enter your choice: ");
                scanf("%d", &roleChoice);

                switch (roleChoice) {
                    case 1:
                        displayInventory(userInventory, userItemCount);
                        break;
                    case 2:
                        generateBill(userInventory, userItemCount);
                    case 3:
                        addToCart(shopkeeperInventory, shopkeeperItemCount, userInventory, &userItemCount);
                        break;
                    case 4:
                        displayInventory(shopkeeperInventory, shopkeeperItemCount);
                        break;
                    case 5:
                        saveInventoryToFile(userInventory, userItemCount, "user_inventory.txt");
                        printf("Exiting program.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            } while (roleChoice != 5);

            break;

        default:
            printf("Invalid role choice. Exiting program.\n");
            break;
    }

    return 0;
}