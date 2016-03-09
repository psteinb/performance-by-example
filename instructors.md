---
layout: page
title: Performance By Example
subtitle: Instructor's Guide
---

## Overall

This lesson is written with an intermediate C++ programmer in mind. Actually it is written with the average _scientist_ (put your industry here) in mind that was just thrown in an PhD project (put your industries 2-3 year project acronym here), using the messy code of his or her predecessor in an environment where speed counts. This lesson is meant to bring this poor individual up to speed with writing modern C++ exploiting available hardware resources, gaining confidence in measuring and inferring about C++ code and yet having fun. 

By far, this lesson makes no attempt to be as comprehensive as the very good text books for C++ that are around (citations needed). It rather tries to collect best practises and teaches them through a bottom-up approach. First a problem is described. Then measurements are conducted if possible that explore the deficiencies in the context of modern computer architecture (profiles are currently only hinted at as their tooling often requires root privileges). And third, a solution is presented that tries to offer performance and good design. Clearly, the material thus lacks a lot of the essential theory behind optimizing code, parallelisation and computer architecture as a whole. But instructors are urged to leave that as is, as the material wants to drag learners in based on their curiosity rather than bust their brains and scare them to death.

Essentially, there is one central message, that the material may want the learners to adapt: That they first measure, then reason about the code and then choose the appropriate technology at hand. At this stage, I am more than happy to discuss this focus in the future.

It aught to be in order to state briefly why C++ is used throughout the material:

* it's a low-level language that offers access to accelerators, CPU registers and other handles of speed.
* at the time of writing, the C++ community is in the midst of a renaissance as the language's standard is revised on a constant basis without loosing any of it's virtues
* it's a very versatile multi-paradigm language both in terms of runtime and compile-time constructs
* it's free, has a lot of (scientific) libraries and is available on all major platforms
* it's very common in HPC and industry

The lesson is taught in an intructor-guided way. An IDE/editor is opened where the students see, the implementation and the tests (possibly the compilation as well) at the same time.  

## Frequently Argued Issues (FAI)

None so far.

## Solutions to exercises

Can be found in dedicated unit test files in the code/ directory of the [repo](https://github.com/psteinb/performance-by-example/tree/gh-pages/code).
