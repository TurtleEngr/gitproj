# pull request template for git-proj

Include the issue number in your pull request.
If you don't have an issue, then go back and create one, giving details.

The code should pass all the checks that are in:
`[doc/hooks/pre-commit](https://github.com/TurtleEngr/gitproj/blob/develop/doc/hooks/pre-commit)`
To make this automatic, put the pre-commit file in your `.git/hooks/`
for this project.  Then add the `[gitproj hooks]` section found in
`[doc/config/gitproj.com](https://github.com/TurtleEngr/gitproj/blob/develop/doc/config/gitproj.config.local)`
to your `~/.gitconfig`.
