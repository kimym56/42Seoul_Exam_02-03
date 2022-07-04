#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone
{
  int width;
  int height;
  char background;
} t_zone;

typedef struct s_shape
{
  char type;
  float x;
  float y;
  // float radius;
  float width;
  float height;
  char color;
} t_shape;

int ft_strlen(char const *str)
{
  int i = 0;
  while (str[i])
    i++;
  return (i);
}
int str_error(char *str)
{
  if (str)
    write(1, str, ft_strlen(str));
  return (1);
}
int clear_all(FILE *file, char *drawing, char *str)
{
  if (file)
    fclose(file);
  if (drawing)
    free(drawing);
  if (str)
    str_error(str);
  return (1);
}

// int in_circle(float x, float y, t_shape *shape)
// {
//   float distance = sqrtf(powf(x - shape->x, 2.) + powf(y - shape->y, 2.));
//   if (distance <= shape->radius)
//   {
//     if ((shape->radius - distance) < 1.00000000)
//       return (2);
//     return (1);
//   }
//   return (0);
// }

   int in_rectangle(float x, float y, t_shape *shape){
   if((x<shape->x || ((shape->x + shape->width) < x)) ||(y<shape->y) || (shape->y + shape->height)<y)
    return 0;
   if(x - shape->x < 1.00000000 || (shape->x + shape->width) - x < 1.00000000 || (y-shape->y) < 1.00000000 || (shape->y + shape->height - y) < 1.00000000)
    return 2;
   return 1;
   }

char  *get_zone(FILE *file, t_zone *zone)
{
  int i;
  char *tmp;

  if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
    return (NULL);
  if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
    return (NULL);
  if (!(tmp = (char *)malloc(sizeof(*tmp) * (zone->width * zone->height))))
    return (NULL);
  i = 0;
  while (i < zone->width * zone->height)
    tmp[i++] = zone->background;
  return (tmp);
}
void draw_shape(char *drawing, t_zone *zone, t_shape *shape)
{
  int y;
  int x;
  int is_it;

  y = 0;
  while (y < zone->height)
  {
    x = 0;
    while (x < zone->width)
    {
      // is_it = in_circle((float)x, (float)y, shape);
      // if ((shape->type == 'c' && is_it == 2) || (shape->type == 'C' && is_it))
      
      is_it = in_rectangle((float)x, (float)y, shape);
      if ((shape->type == 'r' && is_it == 2) || (shape->type == 'R' && is_it))
        drawing[(y * zone->width) + x] = shape->color;
      x++;
    }
    y++;
  }
  // ***************
}
int draw_shapes(FILE *file, char *drawing, t_zone *zone)
{
  int ret;
  t_shape tmp; //
  while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.width, &tmp.height, &tmp.color)) == 6)
  {
    if (tmp.width <= 0.00000000 || tmp.height <= 0.00000000 || (tmp.type != 'r' && tmp.type != 'R'))
      return 0;
    // if (tmp.radius <= 0.000000000 || (tmp.type != 'c' && tmp.type != 'C'))
    //   return (0); //
    draw_shape(drawing, zone, &tmp);
  }
  if (ret != -1)
    return (0);
  return (1);
}
void draw_drawing(char *drawing, t_zone *zone)
{
  int i = 0; //
  while (i < zone->height)
  { //
    write(1, drawing + (i * zone->width), zone->width);
    write(1, "\n", 1);
    i++; //
  }
}

int main(int arg, char **argv)
{
  FILE *file;
  t_zone zone;
  char *drawing;

  zone.width = 0;
  zone.height = 0;
  zone.background = 0;
  drawing = NULL;

  if (arg != 2)
    return (str_error("Error: argument\n"));
  if (!(file = fopen(argv[1], "r")))
    return (str_error("Error: Operation file corrupted\n"));
  if (!(drawing = get_zone(file, &zone)))
    return (clear_all(file, NULL, "Error: Operation file corrupted\n"));
  if (!draw_shapes(file, drawing, &zone))
    return (clear_all(file, drawing, "Error: Operation file corrupted\n"));
  draw_drawing(drawing, &zone);
  clear_all(file, drawing, NULL);
  return (0);
}