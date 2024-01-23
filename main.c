#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <MLX42/MLX42.h>
#include <get_next_line.h>
#include <libft.h>

typedef struct s_mat4 {
    float m[4][4];
} t_mat4;

typedef struct s_vec4 {
    float x;
    float y;
    float z;
    float w;
} t_vec4;

typedef struct s_obj {
    size_t  count;
    size_t  f_count;
    t_vec4  *verts;
    t_vec4  *faces;
} t_obj;


typedef struct s_camera {
    t_mat4  rot;
    t_vec4  ori;
    t_vec4  pos;
    float   ang;
} t_camera;

typedef struct s_state {
    t_obj     objects[2];
    int             *fov;
    float             *a;
    float             *b;
    mlx_t           *mlx;
    mlx_image_t     *image;
    t_camera        *cam;
} t_state;


#define NEAR     1
#define FAR      10

#define WIDTH  800.0f
#define HEIGHT 600.0f

void ft_display_mat(int size, t_mat4 m)
{

    for (int i = 0; i < pow(size, 2); i += 1)
    {
        if (i % size == 0)
            printf("\n");
        printf("%f ", m.m[i/size][i%size]);
    }
    printf("\n");
}

t_mat4 mat4_identity()
{
    t_mat4 m = {0};
    m.m[0][0] = 1.0;
    m.m[1][1] = 1.0;
    m.m[2][2] = 1.0;
    m.m[3][3] = 1.0;
    return (m);
}

t_mat4 mat4_perspective(float ratio, float fov)
{
    t_mat4 m = {0};
    m.m[0][0] = ratio * (1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = FAR / (FAR - NEAR);
    m.m[2][3] = (-FAR * NEAR) / (FAR - NEAR);
    m.m[3][2] = 1.0;
    return (m);
}

t_mat4 mat4_translation(t_vec4 transl)
{
    t_mat4 m = mat4_identity();
    m.m[0][3] = transl.x; 
    m.m[1][3] = transl.y; 
    m.m[2][3] = transl.z;
    return (m);
}

t_mat4 mat4_scale(t_vec4 scale)
{
    t_mat4 m = mat4_identity();
    m.m[0][0] = scale.x;
    m.m[1][1] = scale.y;
    m.m[2][2] = scale.z;
    return (m);
}

t_mat4 mat4_x_mat4(t_mat4 m1, t_mat4 m2)
{
    t_mat4 m = mat4_identity();
    float n = 0;
    for (int j = 0; j < 4; j += 1)
    {
        for (int i = 0; i < 16; i += 1)
        {
            n += m1.m[j][i%4] * m2.m[i%4][i/4];
            if ((i+1)%4==0)
            {
                m.m[j][i/4] = n;
                n = 0;
            }
        }
    }
    return (m);
}

t_mat4 mat4_rot(float a, float b)
{
    t_mat4 m1 = mat4_identity();
    m1.m[1][1] = cos(a);
    m1.m[1][2] = sin(a);
    m1.m[2][1] = -sin(a);
    m1.m[2][2] = cos(a);
    t_mat4 m2 = mat4_identity();
    m2.m[0][0] = cos(b);
    m2.m[0][2] = -sin(b);
    m2.m[1][1] = 1.0; 
    m2.m[2][0] = sin(b);
    m2.m[2][2] = cos(b);
    m2.m[3][3] = 1.0;
    t_mat4 r = mat4_x_mat4(m1, m2); 
    return (r);
}

t_mat4 mat4_rot_vec4(t_vec4 dir, float a)
{
    t_mat4 m1 = mat4_identity();
    m1.m[0][0] = pow(dir.x, 2) + (1 - pow(dir.x, 2)) * cos(a);
    m1.m[0][1] = dir.x * dir.y * (1 - cos(a)) - dir.z * sin(a);
    m1.m[0][2] = dir.x * dir.z * (1 - cos(a)) + dir.y * sin(a);  

    m1.m[1][0] = dir.y * dir.x * (1 - cos(a)) + dir.z * sin(a);
    m1.m[1][1] = pow(dir.y, 2) + (1 - pow(dir.y, 2)) * cos(a);
    m1.m[1][2] = dir.y * dir.z * (1 - cos(a)) - dir.x * sin(a);  

    m1.m[2][0] = dir.z * dir.x * (1 - cos(a)) - dir.z * sin(a);
    m1.m[2][2] = dir.z * dir.y * (1 - cos(a)) + dir.x * sin(a);  
    m1.m[2][1] = pow(dir.z, 2) + (1 - pow(dir.z, 2)) * cos(a);

    return (m1);
}

t_vec4 mat4_x_vec4(t_mat4 m, t_vec4 v)
{
    t_vec4 out = {0};
    out.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    out.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    out.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    out.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    return (out);
}

t_vec4 mat4_x_vec4_proj(t_mat4 m, t_vec4 v)
{
    t_vec4 out = mat4_x_vec4(m, v);
    if (out.w != 0.0)
    {
        out.x = (out.x) / out.w;
        out.y = (out.y) / out.w;
        out.z = (out.z) / out.w;
        out.w = (out.w) / out.w;
    }
    return (out);
}

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

int ft_can_paint(int x0, int y0, int x1, int y1)
{
    if (abs(x0) >= (int)WIDTH / 2 || abs(x1) >= (int)WIDTH / 2)
        return (0);
    if (abs(y0) >= (int)HEIGHT / 2 || abs(y1) >= (int)HEIGHT / 2)
        return (0);
    return (1);
}

void bresenham(int x0, int y0, int x1, int y1, mlx_image_t *image)
{
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    int color = ft_pixel(255, 255, 255, 0xFF);
    while (1)
    {
        int x = x0 + (int)WIDTH / 2;
        int y = y0 + (int)HEIGHT / 2;
        if(ft_can_paint(x0, y0, x1, y1))
        {
            mlx_put_pixel(image, x, y, color);
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * error;
        if (e2 >= dy)
        {
            if (x0 == x1) 
                break;
            error = error + dy;
            x0 = x0 + sx;
        }
        if (e2 <= dx)
        {
            if (y0 == y1) 
                break;
            error = error + dx;
            y0 = y0 + sy;
        }
    }
}

void ft_hook(void* param)
{
    t_state     state = *(t_state*)param;
    t_camera    *cam = state.cam;
    t_obj       c = state.objects[0];
    t_obj       c2 = state.objects[1];
    mlx_t       *mlx = state.mlx;
    mlx_image_t *image = state.image;

    int fov = *(state.fov);
    float fov_rad = (float)fov / 180 * M_PI;
    float ratio = HEIGHT / WIDTH;
    int color = ft_pixel(0xFF, 0xFF, 0xFF, 0xFF);

    t_mat4 projection = mat4_perspective(ratio, fov_rad);
    t_mat4 rotation = mat4_rot(*(state.a), *(state.b));
    t_vec4 scale_vec = (t_vec4){.x = 30, .y = 30, .z = 30};
    // t_mat4 rot = mat4_rot_vec4(ori, 45.0f);
    // t_vec4 cam_pos = mat4_x_vec4(cam->rot, cam->pos); 
    // printf("%f %f %f %f\n", cam_pos.x, cam_pos.y, cam_pos.z, cam_pos.w);
    // t_vec4 t_vec = (t_vec4){.x = 10.0, .y = 20.0 , .z = 1};
    // t_mat4 transl = mat4_translation(t_vec);
    t_mat4 scale = mat4_scale(scale_vec);

    if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
        mlx_close_window(mlx);
    if (mlx_is_key_down(mlx, MLX_KEY_UP))
        *(state.a) += .1;
    if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
        *(state.a) -= .1;
    if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
        *(state.b) += .1;
    if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
        *(state.b) -= .1;
    bzero(image->pixels, ((HEIGHT-1) * image->width + (WIDTH-1)) * sizeof(int32_t)); 
    // for (int i = 1; i < c.count; i += 1)
    // {
    //     // t_vec4 p0 = mat4_x_vec4(projection, mat4_x_vec4(rotation, mat4_x_vec4_proj(scale, c.verts[i-1])));
    //     // t_vec4 p1;
    //     for (int j = 0; j < 2; j += 1)
    //     {
    //         t_vec4 p0 = mat4_x_vec4(projection, mat4_x_vec4(rotation, mat4_x_vec4_proj(scale, c.verts[i-1])));
    //         t_vec4 p1;
    //         // if (j == 0)
    //         // {
    //         //     int n = i + 424 - 1;
    //         //     if (n > c.count)
    //         //         n = c.count-1;
    //         //     p1 = mat4_x_vec4(projection, mat4_x_vec4(rotation, mat4_x_vec4_proj(scale, c.verts[n])));
    //         // }
    //         // else
    //         p1 = mat4_x_vec4(projection, mat4_x_vec4(rotation, mat4_x_vec4_proj(scale, c.verts[i])));
    //
    //         int x0 = p0.x;
    //         int y0 = p0.y;
    //         int x1 = p1.x;
    //         int y1 = p1.y;
    //         // printf("0: %d %d\n", x0, y0);
    //         // printf("1: %d %d\n", x1, y1);
    //         // if (i < c.count - 1 && i % 424 == 0)
    //         //     if (!j)
    //         //         bresenham(x0, y0, x1, y1, image);
    //         if(i % 424 == 0)
    //             continue ;
    //         bresenham(x0, y0, x1, y1, image);
    //     }
    //     int x0 = p0.x;
    //     int y0 = p0.y;
    //     int x1 = p1.x;
    //     int y1 = p1.y;
    //     bresenham(x0, y0, x1, y1, image);
    // }
    for (int i = 0; i < c.f_count; i += 1)
    {
        int f1, f2, f3;
        f1 = c.faces[i].x;
        f2 = c.faces[i].y;
        f3 = c.faces[i].z;
        // printf("points: %d %d %d\n", f1, f2, f3);
        t_vec4 p0 = mat4_x_vec4(projection, mat4_x_vec4(rotation, mat4_x_vec4_proj(scale, c.verts[f1-1])));
        t_vec4 p1 = mat4_x_vec4(projection, mat4_x_vec4(rotation, mat4_x_vec4_proj(scale, c.verts[f2-1])));
        t_vec4 p2 = mat4_x_vec4(projection, mat4_x_vec4(rotation, mat4_x_vec4_proj(scale, c.verts[f3-1])));
        int x0 = p0.x;
        int y0 = p0.y;

        int x1 = p1.x;
        int y1 = p1.y;

        int x2 = p2.x;
        int y2 = p2.y;
        // printf("0: %d %d\n", x0, y0);
        // printf("1: %d %d\n", x1, y1);
        // printf("2: %d %d\n", x2, y2);
        bresenham(x0, y0, x1, y1, image);
        bresenham(x0, y0, x2, y2, image);
        bresenham(x1, y1, x2, y2, image);
    }
}

void ft_free_m(char **m)
{
    int i = 0;
    while (m[i])
        free(m[i++]);
    free(m);
}

int ft_count_vert(char *filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (fd == -1)
        return 0;
    char *line = NULL;
    int vert = 0;
    while ((line = get_next_line(fd)))
        if (line[0] == 'v')
            vert += 1;
    close(fd);
    return (vert);
}

int ft_count_faces(char *filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (fd == -1)
        return 0;
    char *line = NULL;
    int faces = 0;
    while ((line = get_next_line(fd)))
        if (line[0] == 'f')
            faces += 1;
    close(fd);
    return (faces);
}

t_obj ft_parse_obj(char *filepath)
{
    t_obj  obj = { 0 };
    int v = ft_count_vert(filepath);
    if (v == 0)
        return (obj);
    int f = ft_count_faces(filepath);
    obj.verts = malloc(sizeof(t_vec4) * v);
    if (!obj.verts)
        return (obj);
    obj.faces = malloc(sizeof(t_vec4) * f);
    if (!obj.faces)
        return (obj);
    ft_bzero(obj.verts, sizeof(t_vec4) * v);
    ft_bzero(obj.faces, sizeof(t_vec4) * f);
    int fd = open(filepath, O_RDONLY);
    int vertices = 0;
    int faces_c = 0;
    char    *line = NULL;
    while ((line = get_next_line(fd)))
    {
        if (line[0] == 'v')
        {
            char    **vert = ft_split(&line[1], ' ');
            obj.verts[vertices] = (t_vec4) {
                .x = atof(vert[0]),
                .y = atof(vert[1]),
                .z = atof(vert[2])
            };
            ft_free_m(vert);
            vertices += 1;
        }
        else if (line[0] == 'f')
        {
            char    **fc = ft_split(&line[1], ' ');
            t_vec4 q_merda = {
                .x = atoi(fc[0]),
                .y = atoi(fc[1]),
                .z = atoi(fc[2])
            };
            obj.faces[faces_c] = q_merda;
            ft_free_m(fc);
            faces_c += 1;
        }
        free(line);
    }
    close(fd);
    obj.count = v;
    obj.f_count = f;
    return (obj);
}

int main(int ac, char *av[])
{
    t_obj o = ft_parse_obj(av[ac-1]);
    t_obj o2 = ft_parse_obj("data/face.obj");
    t_vec4 camera = (t_vec4) {
        .x = 0,
        .y = 0,
        .z = 5,
        .w = 1
    };
    t_vec4 ori = (t_vec4) {
        .x = 0,
        .y = 1,
        .z = 0,
        .w = 1
    };
    t_camera c = {
        .ang = 45.0f,
        .ori = ori,
        .pos = camera
    };
    // t_cube c = { 0 };
    // c.vert[0] = (t_vec4){.x = -1, .y = -1, .z = 1 + FAR, .w = 1};
    // c.vert[1] = (t_vec4){.x = 1, .y = -1, .z = 1 + FAR, .w = 1};
    // c.vert[2] = (t_vec4){.x = -1, .y = -1, .z = -1 - FAR, .w = 1};
    // c.vert[3] = (t_vec4){.x = 1, .y = -1, .z = -1 - FAR, .w = 1};
    // c.vert[4] = (t_vec4){.x = 1, .y = 1, .z = 1 + FAR, .w = 1};
    // c.vert[5] = (t_vec4){.x = -1, .y = 1, .z = 1 + FAR, .w = 1};
    // c.vert[6] = (t_vec4){.x = 1, .y = 1, .z = -1 - FAR, .w = 1};
    // c.vert[7] = (t_vec4){.x = -1, .y = 1, .z = -1 - FAR, .w = 1};
    t_state state = { 0 };
    state.cam = &c;
    state.objects[0] = o;
    state.objects[1] = o2;
    int initial_fov = 20;
    float a = 35.264;
    float b = 45.0;
    mlx_image_t* image;
    state.fov = &initial_fov;
    state.a = &a;
    state.b = &b;
    mlx_t* mlx;
    if (!(mlx = mlx_init(WIDTH, HEIGHT, "3D render", true)))
        exit (EXIT_FAILURE);
    if (!(image = mlx_new_image(mlx, WIDTH, HEIGHT)))
    {
	mlx_close_window(mlx);
	return(EXIT_FAILURE);
    }
    if (mlx_image_to_window(mlx, image, 0, 0) == -1)
    {
	mlx_close_window(mlx);
	return(EXIT_FAILURE);
    }
    state.mlx = mlx;
    state.image = image;
    mlx_loop_hook(mlx, ft_hook, &state);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return (EXIT_SUCCESS);
}
