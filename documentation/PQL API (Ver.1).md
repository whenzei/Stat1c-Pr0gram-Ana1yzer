# PQL API (Ver.1)

## QueryEvaluator
**Overview** QueryEvaluator will take in a Query object, find relevant information using PKB, and then returns the result to the user.

| **API** | 
| -------- | 
| **LIST<STRING>** GetResultFromQuery(**QUERY** query, **PKB** pkb) <br> ***Description:***  Read the query of the user and get the "declarations entity", "such that" clause and "selection" made by user and
returning a list of results based on the query. If there are no "such that" clauses in the query, this method will use the GetResultFromSelectAllQuery to get the results. If there are no results, return an empty list. | 
| **LIST<STRING>** GetResultFromSelectAllQuery(**STRING** select_var_name,**MAP<STRING, PQLDECLARATIONENTITY>** declarations, **PKB** pkb) <br> ***Description:***  Read the input select_var_name and declarations to know what the user is "selecting" and the list of declarations made by user.
Check if the user has "Selected" a valid variable (that was declared in declarations) and retrieve all item of the variable's 'PqlDeclarationEntity' (e.g "assign") from the PKB, returning the result as a list. If nothing can be found, returns empty list. | 
| **STRING** FormatResultString(**LIST<STRING>** results) <br> ***Description:***  Format the list of results into a readable format to the user. The format varies depending on what the result 'PqlDeclarationEntity' is.
For statement numbers, the format will be "statement #1, #2". For variable or procedures, the format will be "variable/procedure name1, name2". Returns the formatted string to be displayed to the user. | 
