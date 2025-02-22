#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(){
    struct position{
        int x;
        int y;
        char direction;
        char input[101];
    };
    enum{
        North, East, South, West
    };
    struct position robot;
    robot.x = 0;
    robot.y = 0;
    robot.direction = 0;
    while(scanf("%s", robot.input) != EOF){
        for(int i = 0; i < strlen(robot.input); i++){
            if(robot.input[i] == 'G'){
                    switch (robot.direction){
                        case North:
                            robot.y ++;
                            break;
                        case South:
                            robot.y --;
                            break;
                        case West:
                            robot.x --;
                            break;
                        case East:
                            robot.x ++;
                            break;
                        default:
                            break;
                    }
                }
            else{
                if(robot.input[i] == 'R'){
                    robot.direction++;
                }
                else if(robot.input[i] == 'L'){
                    robot.direction--;
                }
                if(robot.direction == 4){
                    robot.direction = 0;
                }
                else if(robot.direction == -1){
                    robot.direction = 3;
                }
            }
        }
        if(robot.x == 0 && robot.y == 0){
            printf("TRUE\n");
        }
        else{
            printf("FALSE\n");
        }
        robot.x = 0;
        robot.y = 0;
    }
return 0;
}