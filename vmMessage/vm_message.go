package vmMessage

const (
	// ERROR 代表resp中的Errorf
	ERROR = iota
	// SimpleString 代表resp中的simplestring
	SimpleString
	// Integer 代表resp中的Integer
	Integer
	// BulkString 代表resp中的BulkString
	BulkString
	// Array 代表resp中的array
	Array
)

// VMMessage 是vm发出的消息类，包含数据库操作的结果类型及详细数据
type VMMessage struct {
	MessageType int
	Details     string
}

// IsError 用来判断消息是否为error
func (message *VMMessage) IsError() bool {
	switch message.MessageType {
	case ERROR:
		return true
	default:
		return false
	}
}

func GenerateErrorMessage(details string) VMMessage {
	return VMMessage{ERROR, details}
}

func GenerateSimpleStringMessage(details string) VMMessage {
	return VMMessage{SimpleString, details}
}

func GenerateBulkStringMessage(details string) VMMessage {
	return VMMessage{BulkString, details}
}
