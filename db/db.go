/*
Package db 实现最基本的数据库引擎，负责具体的数据操作。

操作包括：set,get,existed,del。

*/

package db

import "fmt"

type DbBase struct {
	storage map[string]string
}

func GenerateDbBase() DbBase {
	return DbBase{storage: make(map[string]string)}
}

func (db *DbBase) Set(key, value string) {
	db.storage[key] = value
}

func (db *DbBase) Get(key string) (string, error) {
	ret := db.storage[key]
	if ret == "" {
		return "", DbError{"no such key:" + key}
	}

	return ret, nil
}

func (db *DbBase) Existed(key string) bool {
	if db.storage[key] == "" {
		return false
	}
	return true
}

func (db *DbBase) Del(key string) {
	delete(db.storage, key)
	fmt.Println(db.storage)

}
