---
layout: page
title: Performance By Example
subtitle: Design Choices
---

## Overall

This lesson material is written with an intermediate C++ programmer in mind. Actually it is written with the average _scientist_/_developer_ (or put your industry here) in mind that was just thrown in an PhD project (put your industries 2-3 year project acronym here), using the messy code of his or her predecessor in an environment where speed counts. This lesson is meant to bring this poor individual up to speed with writing modern C++ exploiting available hardware resources, gaining confidence in measuring and inferring about C++ code and yet having fun. 

By far, this lesson makes no attempt to be as comprehensive as the very good text books for C++ that are around (citations needed). It rather tries to collect best practises and teaches them through a bottom-up approach. First a problem is described. Then measurements are conducted if possible that explore the deficiencies in the context of modern computer architecture (profiles are currently only hinted at as their tooling often requires root privileges pr equivalent). And third, a solution is presented that tries to offer performance and good design. Clearly, the material thus lacks a lot of the essential theory behind optimizing code, parallelisation and computer architecture as a whole. But instructors are urged to leave that as is, as the material wants to drag learners in based on their curiosity rather than bust their brains and scare them to death.

What this material really teaches:

* __Use unit tests!__

* __First measure, then reason about the code!__

* __Identify the bottleneck and then choose the appropriate technology at hand!__
        
Why C++ is used throughout the material:

* it's a low-level language that offers access to the compiler, control CPU caches and other handles of speed.
* at the time of writing, the C++ community is in the midst of a renaissance as the language's standard is revised on a constant basis without loosing any of it's virtues
* it's a very versatile multi-paradigm language both in terms of runtime and compile-time constructs
* it's free, has a lot of (scientific) libraries and is available on all major platforms
* it's very common in HPC and industry

The lesson is taught in an intructor-guided way. An IDE/editor is opened where the students see, the implementation and the tests (possibly the compilation as well) at the same time.  

## Assumptions

* Audience

  * graduate students, specialists, developers in any field that uses `C++`
  * who understand the fundamental building plus of the `++` in `C++`
  * who potentially have already written (or are writing) client code applications (in a non-software-engineering way)

* Constraints

  * one full day (9-17h)
      * 6:00 hours of teaching
      * 1:00 hour lunch
      * 1:00 for 2 coffee breaks (can be reduced/split if required)
	  
  * Learners use native installs on their machines (right now Linux is assumed)
	  
  * Presenter has IDE/editor open (1 pane for code, 1 for tests and 1 for test output) 

* Discuss theory on demand and if so only on a conceptual level
  Theoretical digressions will be required (especially to explain why things get faster), they should be kept on a conceptual level to facilitate further study not to bust brains or impress the other sex!

* Throw in "oh this is how I typically do it" to facilitate the look-over-the-experts-shoulder-effect

* Exercises

    * lots of short exercises that can reliably be finished in allotted time
    * MCQs, fill-in-the-blanks, Parsons Problems, "tweak this code", etc.

* Lesson materials  
  Notes for instructors and self-study will be written in Markdown 

## Desired Results

### Goals

1. Immediate

* use unit tests to fix code behavior

* come up with meaningful training data (synthesized at best)

* understand relation between code constructs and hardware architecture

* understand stochastic nature of computers and the impact on the measurement methodology

* simple tools to analyze performance

* implicit/explicit parallelisation


2. Indirect 

* practise to look and understand other people's code

* language and terminology uptake to navigate more easily on online resources and discuss with peers


###
