#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone{
	int width;
	int height;
	char background;
}	t_zone;

typedef struct s_shape{
	char type;
	float x;
	float y;
	float radious; 
	//float width, float height
	char color;
}	t_shape;

int ft_strlen(char *str){
	int i=0;
	while(str[i++]);
	return i;
}
int str_error(char *str){
	if(str)
		write(1,str,ft_strlen(str));
	return (1);
}
int clear_all(FILE *file, char *drawing, char* str){
	if(file)
		fclose(file);
	if(drawing)
		free(drawing);
	if(str)
		str_error(str);
	return (1);
}

int in_circle(float x, float y, t_shape *shape){
	float distance;
	distance = sqrtf(powf(x-shape->x,2.)+powf(y-shape->y,2.));
	if(distance <= shape->radious){
		if(shape->radious - distance < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}
/*
   int in_rectangle(float x, float y, t_shape *shape){
   if((x<shape->x || (shape->x + shape->width) < x)) ||(y<shape->y) || (shape->y + shape->height)<y))
   return 0;
   if((x - shape->x) < 1.00000000 || (shape->x + shape->width) - x < 1.00000000 || (y-shape->y) < 1.00 || (shape->y + shape->height - y) < 1.00
   return 2;
   return 1;
   }
 */
char *get_zone(FILE *file, t_zone *zone){
	char *tmp;
	int i=0;
	if(fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)!=3)
		return (NULL);	// &
	if(zone->width <= 0 || zone->width >300 || zone->height<=0 || zone->height > 300)
		return (NULL);
	if(!(tmp=(char*)malloc(sizeof(*tmp)*(zone->width * zone->height))))
		return (NULL);	// char*
	while(i<zone->width * zone->height){
		tmp[i++] = zone->background;
	}
	return tmp;
}
void draw_shape(char *drawing, t_zone *zone, t_shape *shape){
	// ************
	int x,y,is_it;
	x=0; y=0;
	while(y<zone->height){
		while(x<zone->width){
			is_it = in_circle((float)x, (float)y, shape);
			if((shape->type =='c' && is_it == 2)||(shape->type == 'C' && is_it))
				drawing[y*zone->height + x]=shape->color;
			x++;
		}
		y++;
	}
	// ***************
}
int draw_shapes(FILE *file, char *drawing, t_zone *zone){
	int ret;
	t_shape tmp; //
	while((ret=fscanf(file,"%c %f %f %f %c",&tmp.type,&tmp.x,&tmp.y, &tmp.radious, &tmp.color))==5)
	{
		if(tmp.radious <= 0.000000000 || (tmp.type!='c'&&tmp.type != 'C'))
			return(0);	//
		draw_shape(drawing,zone,&tmp);
	}
	if(ret != -1)
		return (0);
	return (1);
}
void draw_drawing(char *drawing,t_zone *zone){
	int i = 0;//
	while( i< zone->height ){//
		write(1,drawing + i*zone->width,zone->width);
		write(1,"\n",1);
		i++;//
	}
}

int main(int arg, char **argv){
	FILE *file;
	t_zone zone;
	char *drawing;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	drawing = NULL;

	if(arg != 2)
		return (str_error("ERR"));
	if(!(file=fopen(argv[1],"r")))
		return (str_error("ERR2"));
	if(!(drawing=get_zone(file,&zone)))
		return (str_error("ERR2"));
	if(!draw_shapes(file,drawing,&zone))
		return (clear_all(file,drawing,"ERR2"));
	draw_drawing(drawing,&zone);
	clear_all(file,drawing,NULL);
	return(0);
}
