/**
 *  Data file that has vertex and index information for certain objects
 *
 */
namespace DefaultCube
{
    constexpr int cubeVertices = 36;
    //const data setup
    constexpr float vertices[] = {
        //  coords(3)
        //FRONT
        -0.5f,  -0.5f, 0.5f, 
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, 
        0.5f,  0.5f, 0.5f, 

        //BACK
        -0.5f,  -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        -0.5f, 0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f,
    };  

    constexpr float vertRGBCTex[] = {
        //  coords(3)          RGBA(4)                 textureCoord(2)
        //FRONT
        -0.5f,  -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //bottom left
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // bottom right
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left
        0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top right 

        //BACK
        -0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //bottom left
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // bottom right
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top right 
    };  

    constexpr unsigned int indices[] = {  
        //TOP
        1,3,2,
        1,2,0,

        //LEFT
        3,1,5,
        3,5,7,

        //RIGHT
        2,4,0,
        2,6,4,

        //BOTTOM
        7,5,6,
        6,5,4,

        //FRONT
        3,6,2,
        3,7,6,

        //BACK
        1,0,4,
        5,1,4
    };
}
