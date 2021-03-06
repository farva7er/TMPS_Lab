# TMPS_Lab

1. Factory Method:

      Concrete Creator <--> UIManager (See CreateButton() method) (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/UI/UIManager.h)
  
      Product <--> Button  (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/UI/Button.h)
  
      Concrete Product <--> BoxButton (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/UI/Button.h)
  
      Client <--> Sandbox (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/sandbox/src/Sandbox.cpp)
  
2. Object Pool:

      Reusable <--> Resource (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/ResourceManager.h)
  
      Concrete Reusables <--> (Font, Texture, Shader ...)
      
      https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/Renderer/FontRenderer.h
      https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/Renderer/Texture.h
      https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/Renderer/Shader.h
        
      ReusablePool <--> ResourceManager (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/ResourceManager.h)
  
      Example Client <--> Renderer (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/Renderer/Renderer.cpp)
  
 3. Singleton:

      Singleton <--> ResourceManager (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/ResourceManager.h)
  
      Example Client <--> Renderer (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/Renderer/Renderer.cpp)
 
 4. Prototype:
 
      Prototype <--> Animation (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/Animations/Animator.h)
 
      Concrete Prototype <--> InterpAnim (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/Animations/BasicAnimations.h)
 
      Subclass Concrete Prototype <--> LinearInterpAnim (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/totem/src/Animations/BasicAnimations.h)
 
      Example Client <--> Sandbox (https://github.com/farva7er/totem/blob/3887f019ae5deac3c16cc0ddd0ad65cad13d8abe/sandbox/src/Sandbox.cpp)
 
 5. Builder:
 
      Builder <--> Rect::Builder (https://github.com/farva7er/totem/blob/05bd4631b19c5554a8afc03de9986f40d5f231e8/totem/src/Renderer/RendererPrimitives/Rect.h)
 
      Product <--> Rect (https://github.com/farva7er/totem/blob/05bd4631b19c5554a8afc03de9986f40d5f231e8/totem/src/Renderer/RendererPrimitives/Rect.h)
 
      Example Client <--> Sandbox (https://github.com/farva7er/totem/blob/05bd4631b19c5554a8afc03de9986f40d5f231e8/sandbox/src/Sandbox.cpp)
 
 
 For full src of the project see: https://github.com/farva7er/totem 
 
  
  
