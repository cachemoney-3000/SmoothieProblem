#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Info {
    int unit;
    int ratio;
} Info;

typedef struct Recipe {
    int numIngredients;
    int ratio;
    Info* info; 
} Recipe;

typedef struct StoreInfo {
    int recipeNumber;
    int pounds;
} StoreInfo;

typedef struct Store {
    int numRecipesOffered;
    StoreInfo* info;
} Store;

typedef struct Result {
    char* ingredients;
    float needed;
} Result;

FILE *file;
Result* readIngredients(int numIngredients);
Recipe* readRecipe(int numRecipes);
Store* readStore(int numStores);
void printResult(Store* store, Recipe* recipe, Result* result, int numStores, int totalIngredients);
void freeMemory(int numStores, int numRecipes, int numIngredients, Store* store, Recipe* recipe, Result* result);

int main() {
    file  = fopen("in2.txt", "r");

    if(file == NULL) {
        printf("File not found");
        exit(1);
    }

    // Scanning the ingredients
    int numIngredients;
    fscanf(file, "%d", &numIngredients);
    Result* result = readIngredients(numIngredients);
    
    // Scanning the recipes
    int numRecipes;
    fscanf(file, "%d", &numRecipes);
    Recipe* recipe = readRecipe(numRecipes);

    // Scanning the stores
    int numStores;
    fscanf(file, "%d", &numStores);
    Store* store = readStore(numStores);

    
    // Print the final result
    printResult(store, recipe, result, numStores, numIngredients);

    // Free the memory allocated
    freeMemory(numStores, numRecipes, numIngredients, store, recipe, result);

    return 0;
}

Result* readIngredients(int numIngredients){
    // Allocate memory for ingredients
    Result* ingredients = malloc(numIngredients * sizeof(Result));
    int i;

    // Read all the ingredients from the input file line by line
    for (i = 0; i < numIngredients; i++) {
        ingredients[i].ingredients = malloc(20 * sizeof(char));

        char fruits[20];
        fscanf(file, "%s", fruits);
        strcpy(ingredients[i].ingredients, fruits);

        // Initialize what is needed for that ingredient to zero
        ingredients[i].needed = 0;
    }

    return ingredients;
}

Recipe* readRecipe(int numRecipes){
    // Allocate memory for recipe
    Recipe* recipe = malloc(numRecipes * sizeof(Recipe));
    int i;
    int j;

    // Read all the recipes from the input file
    for (i = 0; i < numRecipes; i++) {
        int ingredientCount;
        fscanf(file, "%d", &ingredientCount);

        recipe[i].numIngredients = ingredientCount;
        // Allocate memory for the recipe info 
        recipe[i].info = malloc(ingredientCount * sizeof(Info));

        int totalRatio = 0;
        // Read the recipe info from the input file
        for (j = 0; j < ingredientCount; j++){
            int unit;
            int ratio;
        
            fscanf(file, "%d %d", &recipe[i].info[j].unit, &ratio);

            recipe[i].info[j].ratio = ratio;
            totalRatio = totalRatio + ratio;
        }
        recipe[i].ratio = totalRatio;
    }

    return recipe;
}

Store* readStore(int numStores) {
    // Allocate memory for the store
    Store* store = malloc(numStores * sizeof(Store));
    int i, j;

    // Read what recipes offered in the store
    for (i = 0; i < numStores; i++){
        int recipesOffered;
        fscanf(file, "%d", &recipesOffered);

        store[i].numRecipesOffered = recipesOffered;
        store[i].info = malloc(recipesOffered * sizeof(StoreInfo));

        // Read the recipe number and how many pounds they have
        for (j = 0; j < recipesOffered; j++) {
            fscanf(file, "%d %d", &store[i].info[j].recipeNumber, &store[i].info[j].pounds);
        }
    }

    return store;
}

void printResult(Store* store, Recipe* recipe, Result* result, int numStores, int totalIngredients){
    int x, y, i, j;
    file = fopen("out2.txt", "w");

    // Prints out the store number and the ingredients they need
    for (x = 0; x < numStores; x++) {
        int recipesOffered = store[x].numRecipesOffered;

        for (i = 0; i < recipesOffered; i++) {
        int recipeNumber = store[x].info[i].recipeNumber;
        int pounds = store[x].info[i].pounds;
        int numIngredients = recipe[recipeNumber].numIngredients;
        int totalRatio = recipe[recipeNumber].ratio;


            for (j = 0; j < numIngredients; j++) {
                int u = recipe[recipeNumber].info[j].unit;
                float r = recipe[recipeNumber].info[j].ratio;

                float firstPart = r * pounds;
                float answer = (firstPart / totalRatio);

                result[u].needed = result[u].needed + answer;
            }
    
        }

        // Printing the final result, store by store
        printf("STORE #%d\n", x + 1);
        fprintf(file, "STORE #%d\n", x + 1);
        for (y = 0; y < totalIngredients; y++) {
            if(result[y].needed != 0) {
                printf("%s %f\n", result[y].ingredients, result[y].needed);
                fprintf(file, "%s %f\n", result[y].ingredients, result[y].needed);
                result[y].needed = 0;
            }
        }
        printf("\n");
        fprintf(file, "\n");
    }

    fclose(file);
}

void freeMemory(int numStores, int numRecipes, int numIngredients, Store* store, Recipe* recipe, Result* result) {
    int i;
    // Free memory allocated to store
    for (i = 0; i < numStores; i++){
        free(store[i].info);
    }
    free(store);

    // Free memory allocated to recipe
    for (i = 0; i < numRecipes; i++) {
        free(recipe[i].info);
    }
    free(recipe);

    // Free memory allocated to result
    for (i = 0; i < numIngredients; i++) {
        free(result[i].ingredients);
    }
    free(result);
}