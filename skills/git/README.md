# Git

## Web source
[1] [Git](https://git-scm.com/docs)
[2] [Git commands](https://www.atlassian.com/git/glossary) 
[3] [Top 20 Git Commands With Examples](https://dzone.com/articles/top-20-git-commands-with-examples)

 
 | Command | Description |
 | --- | --- |
 | `$ git config –global user.name “[name] `                                | sets the author name and email address respectively to be used with your commits |  
 | `$ git init [repository name] `                                                     | start a new repository |
 | `$ git clone [url] `                                                                            | obtain a repository from an existing URL |
 | `$ git add [file] `                                                                              | adds a file to the staging area |
 | `$ git commit -m “[ Type in the commit message]” `              | records or snapshots the file permanently in the version history |
 | `$ git commit -a “[ Type in the commit message]” `              | commits any files you’ve added with the git add command and also commits any files you’ve changed since then |
 | `$ git diff`                                                                                             | shows the file differences which are not yet staged |
 | `$ git diff [first branch] [second branch] `                           | shows the differences between the two branches mentioned |
 | `$ git reset [file] `                                                                           | unstages the file, but it preserves the file contents |
 | `$ git reset [commit] `                                                                       | undoes all the commits after the specified commit and preserves the changes locally |
 | `$ git status `                                                                                        | lists all the files that have to be committed |
 | `$ git rm [file] `                                                                                  | deletes the file from your working directory and stages the deletion |
 | `$ git log `                                                                                              | list the version history for the current branch |
 | `$ git log –follow[file] `                                                                  | lists version history for a file, including the renaming of files also |
 | `$ git show [commit] `                                                                          | shows the metadata and content changes of the specified commit |
 | `$ git branch `                                                                                         | lists all the local branches in the current repository |
 | `$ git branch [branch name] `                                                            | creates a new branch |
 | `$ git branch -d [branch name] `                                                      | deletes the feature branch |
 | `$ git checkout [branch name] `                                                        | used to switch from one branch to another |
 | `$ git checkout -b [branch name] `                                                  | creates a new branch and also switches to it | 
 | `$ git merge [branch name] `                                                              | merges the specified branch’s history into the current branch |
 | `$ git remote add [variable name] [Remote Server Link] `    | connect your local repository to the remote server |
 | `$ git push [variable name] master `                                             | sends the committed changes of master branch to your remote repository |
 | `$ git push [variable name] [branch] `                                         | This command sends the branch commits to your remote repository |
 | `$ git push –all [variable name] `                                                 | pushes all branches to your remote repository |
 | `$ git pull [Repository Link] `                                                       | fetches and merges changes on the remote server to your working directory |
 
