# Animation
This is a personal project to create an animation engine.

I'm making my own animation engine not to compete with existing versions but to become a better specialist.

By working on an animation engine, I want to learn about the devils in the details and improve my expertise in next areas:
 - Concrete implementation of advanced math, like linear algebra or any other kind. Math theory is consistent and crystal clear, but concrete implementation of math is a sophisticated task. Just remember `Quake III Arena` fast inverse square root implementation;
 - Usage of optimization techniques for performance-critical code. Evaluation of 3D or 4D vectors fits well into Streaming SIMD Extension registers, like `__m128`, so the engine can evaluate the sum of two vectors in a single instruction and not in four.
 - Technique of Test Driven Development. Using the TDD approach allows to build a sustainable project that catches development errors in places that you don't even think of, but also it takes longer time to implement.
 
Animation Engine classes are in `Animation` project.
Unit Tests for animation classes are in `AnimationUnitTest` project.

Best wishes,
Kirill.
