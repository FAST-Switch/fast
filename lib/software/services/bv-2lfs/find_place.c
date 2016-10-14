#include <stdio.h>
#include <string.h>
#include "xtr2.h"

int * find_place(struct fast_table *fast){
    int i,j,k,m;
    char **r;
    char **r_1;
    u_int16_t * a_delete;
    u_int16_t * a_delete_mask;
    int pla_1,pla_2;
    int * row_place_1;//put the value of && about every addr_vector_1 
    int * row_place_2;
    int * place_1;//put the value of && aboutrow_place_1
    int * place_2;
    int * pla;//put the value of && about place,it`s the final value we want
    
    pla_1 = 0;
    pla_2 = 0;
    
    //************************************************************can be instead***********************
    
    r = (char **)malloc(row*sizeof(char *));
    for(i=0;i<row;i++)
        r[i] = (char *)malloc(10*sizeof(char));
        
    r_1 = (char **)malloc(row*sizeof(char *));
    for(i=0;i<row;i++)
        r_1[i] = (char *)malloc(10*sizeof(char));
        
    a_delete = (u_int16_t *)malloc(row*sizeof(u_int16_t));
    a_delete_mask = (u_int16_t *)malloc(row*sizeof(u_int16_t));
    place_1 = (int *)malloc(row*sizeof(int));
    place_2 = (int *)malloc(row*sizeof(int));
    row_place_1 = (int *)malloc(512*sizeof(int));
    row_place_2 = (int *)malloc(512*sizeof(int));
    pla = (int *)malloc(2*sizeof(int));
    
    
    for(i=0;i<row;i++){
        place_1[i] = 0;
        place_2[i] = 0;
    }
    for(i=0;i<512;i++){
        row_place_1[i] = 0;
        row_place_2[i] = 0;
    }
//***************************************************************************************
    struct_to_char(&(fast->sw_flow_key));
    //strcat(rule_char,sub);
    printf("fast->sw_flow_key:%s\n",rule_char);
    m = 0;
    for(i=0;i<row;i++)
        for(j=0;j<10;j++){
            if(j<9){
               r[i][j] = rule_char[m];
                m++; 
            }
            else
                r[i][j] = '\0';
        }
    for(i=0;i<288;i++)
	rule_char[i] = '\0';
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^here_1^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	//*************************************************************************************** 
    struct_to_char(&(fast->sw_flow_mask));
    //strcat(rule_char,sub);
    m = 0;
    for(i=0;i<row;i++)
        for(j=0;j<10;j++){
            if(j<9){
               r_1[i][j] = rule_char[m];
                m++; 
            }
            else
                r_1[i][j] = '\0';
        }
    for(i=0;i<288;i++)
	rule_char[i] = '\0';
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^here_2^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
//******************************************fore can be insteal****************************

    a_delete = get_new_a(r);
    a_delete_mask = get_mask(r_1);
   
    for(i=0;i<row;i++){
        for(j=0;j<512;j++){
            m = check(j,a_delete_mask[i],a_delete[i]);
            if(m){
               // printf("denug_5\n");
                row_place_1[j] = addr_vector_1[i][j];
                row_place_2[j] = addr_vector_2[i][j];
                if((place_1[i]==0)&&(place_2[i]==0)){
                    place_1[i] = row_place_1[j];
                    place_2[i] = row_place_2[j];
                }
                else{
                    place_1[i] = place_1[i]&row_place_1[j];
                    place_2[i] = place_2[i]&row_place_2[j];
                }
            }
            row_place_1[j] = 0;
            row_place_2[j] = 0;
        }
    }
    
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^here_3^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    pla_1 = place_1[0];
    pla_2 = place_2[0];
    for(i=1;i<row;i++){
        pla_1 = pla_1&place_1[i];
        pla_2 = pla_2&place_2[i];
    }
    
printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^here_4^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    pla[0] = pla_1;
    pla[1] = pla_2;
    return pla;
}
