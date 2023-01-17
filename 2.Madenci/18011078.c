#include <stdio.h>
#include <stdlib.h>

#define MIN_INT -65536
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

typedef struct MaxTuple {
    int left_idx;
    int right_idx;
    int area;
} MaxTuple;

typedef enum MinerState{
    LEFT_MAX, RIGHT_MAX, MIDDLE_MAX 
} MinerState;


MaxTuple bruteForceSolutionToCheck(int * nums);

MinerState findMaxWhich(int a1, int a2, int a3) {
    if(a1 >= a2 && a1 >= a3) {
        return LEFT_MAX;
    }
    if(a2 >= a1 && a2 >= a3) {
        return RIGHT_MAX;
    }
    if(a3 >= a2 && a3 >= a1) {
        return MIDDLE_MAX;
    }
    return MIDDLE_MAX;
}



void extendTuple(MaxTuple* tuple, int* pre, int* suf, int arr_size) {
    while( pre[tuple->left_idx] > 0 && ~tuple->left_idx ) {tuple->left_idx--; }
    while( suf[tuple->right_idx] > 0 && tuple->right_idx < arr_size ) {tuple->right_idx++; } 
    tuple->left_idx++;tuple->right_idx--;
}

int * deepCopy(int * nums) {
    int size = nums[0], i;
    int * copy = (int*) malloc(sizeof(int)* size);
    for(i = 1 ; i < size+1 ; i++) {
        copy[i-1] = nums[i];
    }
    return copy;
}




static int* pre, *suf;
MaxTuple* findMaxDividedAreaOptim(int* A, int L, int R, int size) {

    MaxTuple* max_tuple = (MaxTuple*) malloc(sizeof(MaxTuple));
    int mid = (L + R) / 2;
    
    if(L == R) {
        max_tuple->left_idx = L;
        max_tuple->right_idx = R;
        if(mid == size-1) {
            max_tuple->area = pre[mid];
        }
        else if(mid==0) {
            max_tuple->area = suf[mid];    
        }
        else {
            if(pre[mid-1] + suf[mid] > pre[mid] + suf[mid+1]  )
                max_tuple->area = pre[mid-1] + suf[mid];
            else
                max_tuple->area = pre[mid] + suf[mid+1];
        }
        if (max_tuple->area < 0 || A[mid] > max_tuple->area) {
            max_tuple->area = A[mid];   
        }
        return max_tuple;
    }
    MaxTuple * left_tuple = findMaxDividedAreaOptim(A, L, mid, size); 
    MaxTuple * right_tuple = findMaxDividedAreaOptim(A, mid+1, R, size);

    // MinerState state = find_max_which( left_tuple->area, right_tuple->area, pre[mid] + suf[mid+1] );

    // if(state == LEFT_MAX) {
    //     return left_tuple;
    // }
    // else if(state == RIGHT_MAX) {
    //     return right_tuple;
    // }
    // else {
        
    
    if(left_tuple->area > right_tuple->area) {
        max_tuple->left_idx = left_tuple->left_idx;
        max_tuple->right_idx = left_tuple->right_idx;
        max_tuple->area =  left_tuple->area;
        free(left_tuple);
        return max_tuple;
    } else if (left_tuple->area < right_tuple->area){
        max_tuple->left_idx = right_tuple->left_idx;
        max_tuple->right_idx = right_tuple->right_idx;
        max_tuple->area =  right_tuple->area;
        free(right_tuple);
        return max_tuple;
    } else {
        if(A[mid] > pre[mid] + suf[mid+1]) {
            max_tuple->left_idx=left_tuple->left_idx;
            max_tuple->right_idx=right_tuple->right_idx;
            max_tuple->area = A[mid]; 
        }
        else {
            if( pre[mid] + suf[mid+1] < right_tuple->area) {
                max_tuple->left_idx=left_tuple->left_idx;
                max_tuple->right_idx=left_tuple->right_idx;
                max_tuple->area =  right_tuple->area;
            } else {
                max_tuple->left_idx=left_tuple->left_idx;
                max_tuple->right_idx=right_tuple->right_idx;
                max_tuple->area =  pre[mid] + suf[mid+1];

            }
        }
        return max_tuple;

    }

}

MaxTuple * findMaxAreaToDigOptim(int * nums) {
    int nums_size = nums[0];

    if (nums_size == 1) 
    {
        MaxTuple * tuple = (MaxTuple*) malloc(sizeof(MaxTuple));
        tuple->left_idx = 0;
        tuple->right_idx = 0;
        tuple->area = nums[1];
        return  tuple;
    }
    if (nums_size == 2) {
        if( nums[1] + nums[2] > nums[2] && nums[1] + nums[2] > nums[1] )  {
            MaxTuple * tuple = (MaxTuple*) malloc(sizeof(MaxTuple));
            tuple->left_idx = 0;
            tuple->right_idx = 1;
            tuple->area = nums[1]+ nums[2];
            return  tuple;
        }
        else if (nums[1] > nums[2]) {
            MaxTuple * tuple = (MaxTuple*) malloc(sizeof(MaxTuple));
        
            tuple->left_idx = 0;
            tuple->right_idx = 0;
            tuple->area = nums[1];
            return  tuple; 
        }
        else {
            MaxTuple * tuple = (MaxTuple*) malloc(sizeof(MaxTuple));
            tuple->left_idx = 1;
            tuple->right_idx = 1;
            tuple->area = nums[2];
            return  tuple;
        }
    }
    suf = deepCopy(nums);
    pre = deepCopy(nums);
    int * mine_ground = deepCopy(nums);

    for(int i = 1; i < nums_size; i++)  pre[i] += max(0, pre[i-1]);
    for(int i = nums_size-2; ~i; i--)   suf[i] += max(0, suf[i+1]);

    MaxTuple * tuple =  findMaxDividedAreaOptim(mine_ground, 0, nums_size-1 , nums_size);
    // extendTuple(tuple, pre, suf, nums_size);
    return tuple;
}


int showTestCase(int ** test_arrs, int test_arr_size ) {
    int i, j, test_size;
    for(i = 0 ; i < test_arr_size ; i++) {
        test_size = test_arrs[i][0];
        printf("TEST CASE %d :\n", i);
        for( j = 1 ; j < test_size + 1 ; j++) {
            printf("%d:", test_arrs[i][j]);
        }
        printf("\n");
    }
}

int testCaseValidation(int ** test_arrs, int test_arr_size) {
    int i, j, test_size;
    int wrong_sol_idx = -1;
    for(i = 0 ; i < test_arr_size ; i++) {
        test_size = test_arrs[i][0];
    
        MaxTuple bf_sol = bruteForceSolutionToCheck(test_arrs[i]);
        printf("TEST CASE %d bf solution : %d-%d:%d\n", i, bf_sol.left_idx, bf_sol.right_idx, bf_sol.area );
    
        MaxTuple * dc_sol_optim = findMaxAreaToDigOptim(test_arrs[i]);
        printf("TEST CASE %d dc solution : %d-%d:%d\n", i, dc_sol_optim->left_idx, dc_sol_optim->right_idx, dc_sol_optim->area );
    
        if(bf_sol.area != dc_sol_optim->area) {
            wrong_sol_idx = i;
        }

        free(pre);
        free(suf);

    }
    if(wrong_sol_idx!= -1)
       printf("wrong_answer in %d\n" , wrong_sol_idx);
}


MaxTuple bruteForceSolutionToCheck(int * nums) {
    int i ,j, max=MIN_INT;
    int size = nums[0];
    MaxTuple miner_max_tuple;
    for (i = 1; i < size+1; i++) {
        int tmpMax = 0;
        for(j=i ; j < size+1 ; j++) {
            tmpMax += nums[j];
            if(tmpMax > max) {
                miner_max_tuple.left_idx = i-1;
                miner_max_tuple.right_idx = j-1;
                miner_max_tuple.area = tmpMax;

                max = tmpMax;
            }
        }
    }
    return miner_max_tuple;
}

int main() {
    srand(time(NULL));   // Initialization, should only be called once.
    int i,j;
    int test_size = 21;
    int rand_array_size_bound = 20;
    int random_value_bound = 50; 
    int **test_arrs = (int**) malloc(sizeof(int*)*test_size);

    for (i = 0 ; i < test_size ; i++) {
        int rand_array_size = rand() % rand_array_size_bound+1; 
        int * test = (int*) malloc(sizeof(int) * (rand_array_size + 1));
        test[0] = rand_array_size;
        for(j = 1 ; j < rand_array_size + 1 ; j++) {
            int value = rand() % 50 - 25 ;
            test[j] = value;
        }
        test_arrs[i] = test;
    }
    // int  assignment_test[12] =  {11,8,-30,36,2,-6,52,8,-1,-11,10,4};
    // test_arrs[test_size-1] = assignment_test;
    showTestCase(test_arrs, test_size);

    testCaseValidation(test_arrs, test_size);

    
    int ind_test[16] = {15,-2,1,-12,-25,18,-23,-15,16,7,-23,5,-21,22,-18,3};

    MaxTuple bf_sol = bruteForceSolutionToCheck(ind_test);
    printf("INDIVIDUAL TEST CASE %d bf solution : %d-%d:%d\n", i, bf_sol.left_idx, bf_sol.right_idx, bf_sol.area );

    MaxTuple * dc_sol_optim = findMaxAreaToDigOptim(ind_test);
    printf("INDIVIDUAL TEST CASE %d dc solution : %d-%d:%d\n", i, dc_sol_optim->left_idx, dc_sol_optim->right_idx, dc_sol_optim->area );
    
    
    
}