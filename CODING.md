# Common C/C++ Code Conventions for Qwadro
Coding conventions are essential for maintaining code readability, consistency, and collaboration within a development team. Code that follows industry practices and established guidelines is easier to understand, maintain, and extend. Most projects enforce a consistent style through code conventions. The documentations and examplar projects are no exception. In software-related works and articles designed by SIGMA, you learn our coding conventions and the tools we use to enforce them. You can take our conventions as-is, or modify them to suit your team's needs.

We chose our conventions based on the following goals:
 - Correctness: Our examplars are copied and pasted into your applications. We expect that, so we need to make code that's resilient and correct, even after multiple edits.
 - Teaching: The purpose of our examplars is to teach all of Qwadro, including C and any language needed. For that reason, we don't place restrictions on any language feature or API. Instead, those examplars teach when a feature is a good choice.
 - Consistency: Readers expect a consistent experience across our content. All examplars should conform to the same style.
 - Adoption: We aggressively update our examplars to use new language features. That practice raises awareness of new features, and makes them more familiar to all C developers.

## Introduction
These guidelines are used by SIGMA to develop examplars and documentation. They were adopted from several guidelines including C/C++ forward coding, high-performance computing, computer graphics, embedded programming and data-oriented programming. We chose those guidelines because they have been tested over several years of Open Source development. They've helped community members participate in the runtime and compiler projects. They are meant to be an example of common C conventions for Qwadro, and not an authoritative list.

The teaching and adoption goals are why the docs coding convention differs from the runtime and compiler conventions. Both the runtime and compiler have strict performance metrics for hot paths. Many other applications don't. Our teaching goal mandates that we don't prohibit any construct. Instead, examplars show when constructs should be used. We update examplars more aggressively than most production applications do. Our adoption goal mandates that we show code you should write today, even when code written last year doesn't need changes.

This article explains our guidelines. The guidelines have evolved over time, and you'll find examplars that don't follow our guidelines. We welcome PRs that bring those examplars into compliance, or issues that draw our attention to examplars we should update. Our guidelines are Open Source and we welcome PRs and issues. However, if your submission would change these recommendations, open an issue for discussion first. You're welcome to use our guidelines, or adapt them to your needs.

## Language guidelines
The following sections describe practices that the documentation team follows to prepare code examples and samples. In general, follow these practices:
 - Utilize modern language features and C# versions whenever possible.
 - Avoid obsolete or outdated language constructs.
 - Write code with clarity and simplicity in mind.
 - Avoid overly complex and convoluted code logic.

More specific guidelines follow.

 - Use object initializers to simplify object creation.

## Style guidelines
In general, use the following format for code samples:
 - Use four spaces for indentation. Don't use tabs.
 - Align code consistently to improve readability.
 - Limit lines to 65 characters to enhance code readability on docs, especially on mobile screens.
 - Break long statements into multiple lines to improve clarity.
 - Use the "Allman" style for braces: open and closing brace its own new line. Braces line up with current indentation level.
 - Line breaks should occur before binary operators, if necessary.
 - Use extra paranthesis to enforce clarity in PEMDAS (Parentheses, Exponents, Multiplication/Division, Addition/Subtraction) rules.

## Comment style
 - Use single-line comments (//) for brief explanations.
 - Do not use multi-line comments (/* */) for single-line comments.
 - For describing methods, classes, fields, and all public members use structured comments.
 - Place the comment on a separate line, not at the end of a line of code.
 - Begin comment text with an uppercase letter.
 - End comment text with a period.
 - Insert one space between the comment delimiter (//) and the comment text, as shown in the following example.

## Layout conventions
Good layout uses formatting to emphasize the structure of your code and to make the code easier to read. SIGMA examples and samples conform to the following conventions:
 - Use the default Code Editor settings (smart indenting, four-character indents, tabs saved as spaces).
 - Write only one statement per line.
 - Write only one declaration per line.
 - If continuation lines aren't indented automatically, indent them one tab stop (four spaces).
 - Add at least one blank line between method definitions and property definitions.
 - Use parentheses to make clauses in an expression apparent, as shown in the following code.
 
