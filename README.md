# 2ch
### 2챕터 TextRPG 3조




# 프로젝트 시작

```
1. git clone https://github.com/MurRuck/TextRPG-C-

```





# Git branch 전략

## main
개발 기준 브랜치
모든 서브 브랜치는 main 기준으로 branch 생성

## sub-branch
```bash
git switch -c <sub-branch-name>

코드를 수정
1. git add  >> 커밋을 위한 파일 추가
2. git commit -m "커밋메시지" >> 클라우드 서버에 올리기 위한 확정단계
3. git push -u origin/main >> git push  >> 커밋된 파일을 클라우드서버로 업로드 하겠다.
4. git pull
```
## 서브 브랜치 네이밍
- feat/~
> feature, 기능구현 시

- fix/~
> fix, 기능 및 버그 수정 시

- chore/~
> 빌드 설정, 의존성 추가/삭제/수정 시

- docs/~
> 문서 추가/삭제/수정 시 

- refac/~
> 폴더구조, 기능 등 리팩토링 시

- ci/~
> workflow 수정 시

- hotfix/~
> 즉시 main branch merge 필요할 경우



# Git commit message

## Commit message 네이밍

- feat: ~
> feature, 기능구현 시

- fix: ~
> fix, 기능 수정 시

- chore: ~
> 빌드 설정, 의존성 추가/삭제/수정 시

- docs: ~
> 문서 추가/삭제/수정 시 

- refac/~
> 폴더구조, 기능 등 리팩토링 시

- ci: ~
> workflow 수정 시

- hotfix: ~
> 즉시 main branch merge 필요할 경우

