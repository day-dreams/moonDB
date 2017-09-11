package db

type DbError struct {
	Operation string
}

func (e DbError) Error() string {
	return "db error: " + e.Operation
}
