#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_RECORDS 211

typedef struct question1 //Struct to store all values from file
{
	int ref_date; //year;
	char geo[50]; //country/province
	char age_group[50]; //the age group
	char sex[50]; //gender
	double value; //% of pop. that has diabetes
} question1;

typedef struct average //Struct to store all values for the average
{
	double avg;  //average for all years and age groups
	double values;  //sum of all percentages 
	double num;  //total number of values entered
} average;

//Fuunction to remove quotes from the data in the file
void removeQuotes(char* raw, char* result){
	int len = strlen(raw);
	strncpy(result, raw + 1, len - 2);
	result[len - 2] = '\0';
}

//Function to count the number of data values and their sum for each geo and year
//values / num = average
void
 getAverage(average arrAverage[5][8], int x, int ref_date, double value){
	arrAverage[x][0].num++;
	arrAverage[x][0].values += value;
		
	if(ref_date == 2015){
		arrAverage[x][1].num++;
		arrAverage[x][1].values += value;
	}
	else if(ref_date == 2016){
		arrAverage[x][2].num++;
		arrAverage[x][2].values += value;
	}	
	else if(ref_date == 2017){
		arrAverage[x][3].num++;
		arrAverage[x][3].values += value;
	}	
	else if(ref_date == 2018){
		arrAverage[x][4].num++;
		arrAverage[x][4].values += value;
	}
	else if(ref_date == 2019){
		arrAverage[x][5].num++;
		arrAverage[x][5].values += value;
	}
	else if(ref_date == 2020){
		arrAverage[x][6].num++;
		arrAverage[x][6].values += value;
	}
	else if(ref_date == 2021){
		arrAverage[x][7].num++;
		arrAverage[x][7].values += value;
	}	
}

//Sorts the provincial average array to obtain the highest and lowest averages
void
sortAvg (double provAvg[]) {
   double temp;
   
   for(int j = 0; j < 3; j++){
	   for(int i = 0; i < 3; i++){
			if(provAvg[i+1] < provAvg[i]){
				temp = provAvg[i];
				provAvg[i] = provAvg[i+1];
				provAvg[i+1] = temp;
			}
		}
	}
}

//Determines the lowest and highest average for each year and province
void
sortArrAvg(average arrAverage[5][8], char provH[20], int *yearH, char provL[20], int *yearL){

	double highestAvg = arrAverage[1][1].avg;
	double lowestAvg = arrAverage[1][1].avg;
	
	for(int i = 1; i < 5; i++){
		for(int j = 1; j < 8; j++){
			
			//Highest Average
			if(arrAverage[i][j].avg > highestAvg){
				highestAvg = arrAverage[i][j].avg;
				
				if(i == 1)
					strncpy (provH, "- Quebec", 20);
				else if(i == 2)
					strncpy (provH, "- Ontario", 20);
				else if(i == 3)
					strncpy (provH, "- Alerta", 20);
				else if(i == 4)
					strncpy (provH, "- British Columbia", 20);
				
				if(j == 1)
					*yearH = 2015;
				else if(j == 2)
					*yearH = 2016;
				else if(j == 3)
					*yearH = 2017;
				else if(j == 4)
					*yearH = 2018;
				else if(j == 5)
					*yearH = 2019;
				else if(j == 6)
					*yearH = 2020;
				else if(j == 7)
					*yearH = 2021;
			}
			
			//Lowest average
			if(arrAverage[i][j].avg < lowestAvg){
				lowestAvg = arrAverage[i][j].avg;
				
				if(i == 1)
					strncpy (provL, "- Quebec", 20);
				else if(i == 2)
					strncpy (provL, "- Ontario", 20);
				else if(i == 3)
					strncpy (provL, "- Alerta", 20);
				else if(i == 4)
					strncpy (provL, "- British Columbia", 20);
				
				if(j == 1)
					*yearL = 2015;
				else if(j == 2)
					*yearL = 2016;
				else if(j == 3)
					*yearL = 2017;
				else if(j == 4)
					*yearL = 2018;
				else if(j == 5)
					*yearL = 2019;
				else if(j == 6)
					*yearL = 2020;
				else if(j == 7)
					*yearL = 2021;
			
			}
		}
	}	
}	

int main(){
	FILE* file; //opens file
	file = fopen ("statscan_diabetes.csv", "r");//open file;
	
	//if file doesnt open
	if (file == NULL){
		perror("Error");
		return 1;
	}
	
	//Defining all variables 
	question1 arr[MAX_RECORDS];
	int records = 0;
	int initalize = 0;
	int ref_date_num = 0;
	int geo_num = 1;
	int age_num = 3;
	int sex_num = 4;
	int value_num = 13;
	char line[4096];
	char provH[20];
	char provL[20];
	int yearH;
	int yearL;
	
	//Creating an average struct for each average needed
	//Geos
	average ca;
	average on;
	average qu;
	average bc;
	average al;
	
	//Age groups
	average age1; //35 to 49 years
	average age2; //50 to 64 years
	average age3; //65 years and over
	
	//Years
	average total;
	average y15;
	average y16;
	average y17;
	average y18;
	average y19;
	average y20;
	average y21;
	
	//makes a 2D array of stucts for averages of all years for each province 
	average arrAverage[5][8] = {{ca, qu, on, al, bc},{total, y15, y16, y17, y18, y19, y20, y21}};  
	
	//Loop to obtain data from file
	while (fgets(line, 4096, file)){
		
		//exit loop if file is read
		if(records == MAX_RECORDS) 
		    break;
		
		// printf("%s\n", buf);
		if (!initalize){
			initalize++;
			records++;
			continue;
		}

		question1 temp;

		// loop through all columns until equal to "," signifying end of column
		// value at current column put into item
		// item = "2015"
		char *token;
		token = strtok(line, ",");
		int col = 0;
		while (token != NULL)
		{// do whatever on the string
			char result[1000];
			removeQuotes(token, result);

			if (col == geo_num) //if in the geo column
				strcpy(temp.geo, result); //store in geo struct
			else if (col == ref_date_num) //if in the ref date column
				temp.ref_date = atoi(result); //store in the ref date struct
			else if (col == age_num)
				strcpy(temp.age_group, result);
			else if (col == sex_num)
				strcpy(temp.sex, result);
			else if (col == value_num)
				temp.value = atof(result);
			
			token = strtok(NULL, ",");
			col++;
			// points to start of next column
		}
		
		arr[records] = temp;
		records++;
		
		//Un-comment line below to print all raw data values from the file
		//printf("%d | %s | %s | %s | %.2lf \n", temp.ref_date, temp.age_group, temp.geo, temp.sex, temp.value);
		
		//averages for country/province -- 1a, 1b, 1c
		//arrAverage[0][0] = Canada for all years
		//arrAverage[i][j] 
		//i = each geo
		//j = each year
		if(strcmp( temp.geo, "Canada (excluding territories)") == 0)
			getAverage(arrAverage, 0, temp.ref_date, temp.value);
		
		//For Quebec 
		else if(strcmp( temp.geo, "Quebec") == 0)
			getAverage(arrAverage, 1, temp.ref_date, temp.value);
		
		//For Ontario
		else if(strcmp( temp.geo, "Ontario") == 0)
			getAverage(arrAverage, 2, temp.ref_date, temp.value);
		
		//For Alberta
		else if(strcmp( temp.geo, "Alberta") == 0)
			getAverage(arrAverage, 3, temp.ref_date, temp.value);
		
		//For British Columbia
		else if(strcmp( temp.geo, "British Columbia") == 0)
			getAverage(arrAverage, 4, temp.ref_date, temp.value);


		//averages for age group -- 1d
		if(strcmp( temp.age_group, "35 to 49 years") == 0){
			age1.num ++;
			age1.values += temp.value;
		}
		else if(strcmp( temp.age_group, "50 to 64 years") == 0){
			age2.num ++;
			age2.values += temp.value;
		}
		else if(strcmp( temp.age_group, "65 years and over") == 0){
			age3.num ++;
			age3.values += temp.value;
		}

	}
	fclose(file);

	//calculating averages of each geo for all years
	ca.avg = arrAverage[0][0].values / arrAverage[0][0].num;
	qu.avg = arrAverage[1][0].values / arrAverage[1][0].num;
	on.avg = arrAverage[2][0].values / arrAverage[2][0].num;
	al.avg = arrAverage[3][0].values / arrAverage[3][0].num;
	bc.avg = arrAverage[4][0].values / arrAverage[4][0].num;
	
	//calculating averages of each age group
	age1.avg = age1.values / age1.num;
	age2.avg = age2.values / age2.num;
	age3.avg = age3.values / age3.num;
	
	//calculating averages of each geo for each year
	for(int i = 0; i < 5; i++){
		for(int j = 1; j < 8; j++){
			arrAverage[i][j].avg = arrAverage[i][j].values / arrAverage[i][j].num;
			//printf("%.2lf\n",arrAverage[i][j].avg);
		}
	}

//Outputing all averages for each question:
//Question 1
	//Printing averages
	printf("-All Values are Percentages-\n");
	printf("\nQUESTION 1\n\n");
	
	//Averages for each province
	printf("1a) Provincial Averages \n"); 
	printf("- Quebec: %.2lf\n", qu.avg);
	printf("- Ontario: %.2lf\n", on.avg);
	printf("- Alberta: %.2lf\n", al.avg);
	printf("- British Columbia: %.2lf\n", bc.avg);
	printf("\n");
	
	// Average for all of Canada
	printf("1b) National Average\n");
	printf("- Canada: %.2lf\n\n", ca.avg);
	
	
	//Averages of each province for every year
	
	//opens files for graph 1
	FILE* data1ca;
	data1ca = fopen("data1ca.txt", "w");
	
	FILE* data1qu;
	data1qu = fopen("data1qu.txt", "w");
	
	FILE* data1on;
	data1on = fopen("data1on.txt", "w");
	
	FILE* data1al;
	data1al = fopen("data1al.txt", "w");
	
	FILE* data1bc;
	data1bc = fopen("data1bc.txt", "w");
	
	printf("1c) Yearly Average for each Province");
	char geo[20];
	int year;
	for(int i = 0; i < 5; i++){
		printf("\n");
		for(int j = 1; j < 8; j++){
			
			if(i == 0)
				strcpy(geo, "Canada");
			else if(i == 1)
				strcpy(geo, "Quebec");
			else if(i == 2)
				strcpy(geo, "Ontario");
			else if(i == 3)
				strcpy(geo, "Alberta");
			else if(i == 4)
				strcpy(geo, "British Columbia");	
			
			if(j == 1)
				year = 2015;
			else if(j== 2)
				year = 2016;
			else if(j== 3)
				year = 2017;
			else if(j== 4)
				year = 2018;
			else if(j== 5)
				year = 2019;
			else if(j== 6)
				year = 2020;
			else if(j== 7)
				year = 2021;
	
			printf("- %s, %i: %.2lf\n", geo, year, arrAverage[i][j].avg);
			
			//Adds the average per year for each graph
			if(i == 0)
				fprintf(data1ca, "%s %i %.2lf\n", geo, year, arrAverage[i][j].avg);
			else if(i == 1)
				fprintf(data1qu, "%s %i %.2lf\n", geo, year, arrAverage[i][j].avg);
			else if(i == 2)
				fprintf(data1on, "%s %i %.2lf\n", geo, year, arrAverage[i][j].avg);
			else if(i == 3)
				fprintf(data1al, "%s %i %.2lf\n", geo, year, arrAverage[i][j].avg);
			else if(i == 4)
				fprintf(data1bc, "%s %i %.2lf\n", geo, year, arrAverage[i][j].avg);
				
		}
	}
	printf("\n");
	
	//average for each age group
	FILE* data2;
	data2 = fopen("data2.txt", "w");
	
	printf("1d) Average per Age Group\n");
	printf("- 35-49: %.2lf\n", age1.avg);
	printf("- 60-64: %.2lf\n", age2.avg);
	printf("- 65+: %.2lf\n", age3.avg);
	
	fprintf(data2, "0 35-49 %.2lf\n", age1.avg);
	fprintf(data2, "1 60-64 %.2lf\n", age2.avg);
	fprintf(data2, "2 65+ %.2lf\n", age3.avg);
			
// end of Question 1
	
//Question 2 
	printf("\nQUESTION 2\n");
	double provAvg[4] = {qu.avg, on.avg, al.avg, bc.avg};
	sortAvg(provAvg);
	
	//Prov with lowest percent
	printf("\nProvince with lowest percentage of diabetes: \n");
	if(provAvg[0] == qu.avg)
		printf("- Quebec\n");
	
	else if(provAvg[0] == on.avg)
		printf("- Ontario\n");

	else if(provAvg[0] == al.avg)
		printf("- Alberta\n");
	
	else if(provAvg[0] == bc.avg)
		printf("- British Columbia\n");
	
	
	//Prov with higheset percent
	printf("\nProvince with highest percentage of diabetes: \n");
	if(provAvg[3] == qu.avg)
		printf("- Quebec\n");
		
	else if(provAvg[3] == on.avg)
		printf("- Ontario\n");
	
	else if(provAvg[3] == al.avg)
		printf("- Alberta\n");
	
	else if(provAvg[3] == bc.avg)
		printf("- British Columbia\n");
	
// end of Question 2

//Question 3
	printf("\nQUESTION 3\n");
	
	printf("\nProvinces above national average:\n");
	if(qu.avg > ca.avg)
		printf("- Quebec\n");
	if(on.avg > ca.avg)
		printf("- Ontario\n");
	if(al.avg > ca.avg)
		printf("- Alberta\n");
	if(bc.avg > ca.avg)
		printf("- British Columbia\n");
	
	printf("\nProvinces below national average:\n");
	if(qu.avg < ca.avg)
		printf("- Quebec\n");
	if(on.avg < ca.avg)
		printf("- Ontario\n");
	if(al.avg < ca.avg)
		printf("- Alberta\n");
	if(bc.avg < ca.avg)
		printf("- British Columbia\n");
// end of Question 3

//Question 4		
	printf("\nQUESTION 4\n");
	sortArrAvg(arrAverage, provH, &yearH, provL, &yearL);
	
	printf("\nProvince and year with higest percent of diabetes:\n");
	printf("%s, %i\n", provH, yearH);
	
	printf("\nProvince and year with lowest percent of diabetes:\n");
	printf("%s, %i\n", provL, yearL);
// end of Question 4

	return (0);
}
